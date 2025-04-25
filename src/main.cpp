#include <memory>

#include "sensesp.h"
#include "sensesp_app_builder.h"

#include "shared.h"
#include "rudder_angle_sensor.h"

using namespace sensesp;


// The setup function performs one-time application initialization.
void setup() {
  SetupLogging(ESP_LOG_DEBUG);

  SensESPAppBuilder builder;
  sensesp_app = (&builder)
    ->set_hostname("symphony-sensesp-aft")
    ->set_sk_server("Marks-MacBook-Pro.local", 3000)
    //->set_sk_server("signalk.local", 3000)
    ->enable_system_info_sensors("sensors.")
    ->get_app();

  auto rudderAngleSensor = std::make_shared<RudderAngleSensor>();

  while (true) {
    loop();
  }
}

void loop() { event_loop()->tick(); }

std::vector<std::shared_ptr<void>> retained = std::vector<std::shared_ptr<void>>();