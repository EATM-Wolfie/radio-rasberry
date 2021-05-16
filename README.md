

# radio-rasberry

This is the little program which generates random music if someone walks past the PIR sensor

# to build the file

run `./make.sh`. this outputs a file called `wav`. this is the file to run

# the path is important!

this must go into the following path: ```/home/pi/src/radio-rasberry``` (Note the typo!)

to run this as a service the path of the program has to be correct!

## The service script.

Write this script and save as ```/etc/systemd/system/radio.service```

    #The service script


    # Contents of /etc/systemd/system/myservice.service
    [Unit]
    Description=EATM Radio
    # After=network.target

    [Service]
    Type=simple
    Restart=always
    ExecStart=/home/pi/src/radio-rasberry/wav

    [Install]
    WantedBy=multi-user.target
    
    
Once done, run the following commands

    systemctl daemon-reload
    systemctl start radio
    systemctl status radio
        
The last command will show you if it is running OK

if you want it so the service is enabled on power up (even before login), run

    systemctl enable radio
        
You have to do all of these as ```sudo```


