# Sleep light for my son

My son can't read the clock yet. If he wakes up in the night, he doesn't know if it is still sleeping time, or if it's ok to come over to the parent's bed to cuddle.

So I made a simple traffic light with 3 LED (red -> sleep, yellow -> cuddle, green -> get up). The color is set by MQTT (RED, YELLOW, GREEN) and controlled by an automation in my Home Assistant.

## Home Assistant 

The automation is quite simle. I created three datetime_input helper (time only) holding the beginning times for red/yellow/green phases. The automation is called periodically and pushes the current color to an MQTT topic. The ESP8266 receives the color and switches the corresponding LED.

```yaml
alias: Schlafampel
description: Sendet alle 5 min den Trigger zur Aktualisierung der Schlafampel
trigger:
  - platform: time_pattern
    minutes: /5
condition: []
action:
  - if:
      - condition: time
        after: input_datetime.tom_timer_rot_nach
        before: input_datetime.tom_timer_gelb_nach
    then:
      - service: mqtt.publish
        data:
          topic: objekt/sleep_traffic_light/color
          payload: RED
          retain: true
          qos: "1"
    else:
      - if:
          - condition: time
            after: input_datetime.tom_timer_gelb_nach
            before: input_datetime.tom_timer_gruen_nach
        then:
          - service: mqtt.publish
            data:
              qos: "1"
              topic: objekt/sleep_traffic_light/color
              payload: YELLOW
              retain: true
        else:
          - service: mqtt.publish
            data:
              qos: "1"
              topic: objekt/sleep_traffic_light/color
              payload: GREEN
              retain: true
mode: single


```
