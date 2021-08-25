Set time into RTC accurately
============================

 - Step 0: Connect RTC to device (pins 2 and 3 on Pro Micro)
 - Step 1: Flash to device by running `pio run -t upload`
 - Step 2: Run `python send_timestamp_to_RTC.py`
    - Note: May need _pyserial_ (`pip install --user pyserial`)

Enjoy accurate RTC
