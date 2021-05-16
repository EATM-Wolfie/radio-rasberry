

# radio-rasberry

This is the little program which generates random music if someone walks past the PIR sensor

# to build the file

run `./make.sh`. this outputs a file called `wav`. this is the file to run

# the path is important!

this must go into the following path: ```/home/pi/src/radio-rasberry``` (Note the typo!)

to run this as a service the path of the program has to be correct!

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
