# Aft sensesp device for S/V Symphony
This is for the aft-ward sensesp unit installed in S/V Symphony, part of Dark Star LLC's fleet.

It has these proposed responsibilities:
* Send readings for `steering.rudderAngle` to Sympony's signalk instance.
* Switch relays for smart stern light, binnacle light, cockpit lights, etc.
* Switch relay for an audible piezo alarm, for a variety of alarm conditions
* To transmit button presses for:
  * MOB (with a long press)
  * air horn
  * autopilot remote controls (toggle on/off,  +10 +1 -1 -10, mode(s))
  * lighting changes that might be done automatically and could overriden (e.g. force windex light off)
  * overriding lighting for these signalk values in order to force certain nav light configurations:
    * `navigation.state` 
    * `environment.mode`
 * possibly, connecting to the drive motor for pypilot rudder control (if not using a dedicated arduino nano)
 * possibly, driving a small e-ink screen for autopilot info, or as a general MFD

# SensESP

This project was generated using the template for [SensESP](https://github.com/SignalK/SensESP/) projects. An proof of concept for just the rudder angle sensor can be found at [sensesp-rudder-angle-sensor](https://github.com/mark-brannan/sensesp-rudder-angle-sensor); that example code has been adapted for S/V Symphony's aft sensesp unit.

Comprehensive documentation for SensESP, including how to get started with your own project, is available at the [SensESP documentation site](https://signalk.org/SensESP/).
