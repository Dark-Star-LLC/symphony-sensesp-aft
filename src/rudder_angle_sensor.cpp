#include <memory>
#include <functional>

#include "sensesp/sensors/sensor.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp/transforms/linear.h"
#include "sensesp/transforms/voltagedivider.h"
#include "sensesp/ui/status_page_item.h"

#include "shared.h"
#include "linear.h"
#include "radians.h"
#include "rudder_angle_sensor.h"

using namespace sensesp;

using xyPair = std::pair<float, float>;

// https://signalk.org/specification/1.4.0/doc/vesselsBranch.html#vesselsregexpsteeringrudderangle
const char* sk_path = "steering.rudderAngle";
const char* kUIGroup = "Rudder Angle Sensor";

// GPIO number to use for the analog input
const uint8_t kAnalogInputPin = 36;
// Define how often (in milliseconds) new samples are acquired
const unsigned int kAnalogInputReadInterval = 500;
const float kAnalogInputScale = 3.3;
const float kFixedResistorValue = 47; // Ohms

// Using measured min/max resistance values through esp32 ADC and sensesp;
// which differ by ~10~20 Ohms compared with externally verified values (0-190 Ohms),
const float kMinSensorResistance = 2.113363;
const float kMaxSensorResistance = 223.;

const float minSensorDegrees = -50.0;
const float maxSensorDegrees = 50.0;

RudderAngleSensor::RudderAngleSensor() {
  analogSetPinAttenuation(kAnalogInputPin, ADC_ATTENDB_MAX);

  auto analog_input = std::make_shared<RepeatSensor<float>>(kAnalogInputReadInterval, []() {
    return analogReadMilliVolts(kAnalogInputPin) / 1000.;
  });

  analog_input->attach([analog_input]() {
    debugD("Rudder angle sensor analog input value: %f", analog_input->get());
  });

  auto voltageDivider = std::make_shared<VoltageDividerR2>(
    kFixedResistorValue, kAnalogInputScale, "/Sensors/Rudder Angle/VoltageDividerR2");

  voltageDivider->attach([voltageDivider]() {
    debugD("Rudder angle sensor resistance value: %f", voltageDivider->get());
  });

  auto transformToDegrees = linearTransformOf(
      xyPair(kMinSensorResistance, minSensorDegrees),
      xyPair(kMaxSensorResistance, maxSensorDegrees)
    );

  transformToDegrees->attach([transformToDegrees]() {
    debugD("Rudder angle sensor degrees value: %f", transformToDegrees->get());
  });

  auto degreesToRadians = std::make_shared<RadiansTransform>();

  degreesToRadians->attach([degreesToRadians]() {
    debugD("Rudder angle sensor radians value: %f", degreesToRadians->get());
  });


  auto sk_output = std::make_shared<SKOutput<float>>(sk_path, "",
    std::make_shared<SKMetadata>("rad", "Rudder Angle"));

  analog_input
    ->connect_to(voltageDivider)
    ->connect_to(transformToDegrees)
    ->connect_to(degreesToRadians)
    ->connect_to(sk_output);

  retain(analog_input);
  retain(sk_output);

  auto makeStatusPageItemFor = [](const char* name, int order) {
    auto status_page_item = std::make_shared<StatusPageItem<float>>(name, -1., kUIGroup, order);
    retain(status_page_item);
    return status_page_item;
  };

  analog_input->connect_to(makeStatusPageItemFor("analog input", 1));
  voltageDivider->connect_to(makeStatusPageItemFor("voltage divider conversion to resistance", 2));
  transformToDegrees->connect_to(makeStatusPageItemFor("linear conversion to degrees", 3));
  degreesToRadians->connect_to(makeStatusPageItemFor("conversion to radians", 4));
  sk_output->connect_to(makeStatusPageItemFor("Value sent to SK for 'steering.rudderAngle'", 5));
};