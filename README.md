#Laser Tag Reloaded - System

---

##What is this

This is a laser tag system which connects to your android smartphone using bluetooth. Your smartphone then connects to a remote server which handles tracking the lobby and game data. 

The program flow is simple: You pair the smartphone with the gun/vest, you join or create a lobby, and then you play! 

Each player is assigned a number, which gets transmitted by the infrared laser. When another player receives a valid enemy number it transmits this to the smartphone, and then the smartphone transmits it to the server.

When the end game conditions are met, the server nofies every player. At the end of each game you can see a map with markers showing the places where you got shot. 

---

###Project Links

The project has three components: 

* [__System__ (Gun + Vest)](https://github.com/cesarandreu/laser_tag_reloaded)

* [__Server__ (Node.js) ](https://github.com/cesarandreu/laser_tag_nodejs)

* [ __Smartphone application__ (Android)](https://github.com/cesarandreu/laser_tag_android)

Videos

* [System demonstration](http://www.youtube.com/watch?v=hs2FppfotFk)

---

###System


The system (gun + vest) has an infrared laser, two infrared receivers, a speaker, a bluetooth module, a GPS module, a battery gauge, a battery charger, a battery, an LED bar graph, and two push-buttons. 

The essential components are: the infrared laser, the infrared receivers, the speaker, the bluetooth, and the push-button. (And a power source.) These components are required for the system to function. You could remove the LED graph, second button, battery charger, battery gauge, and GPS, and still maintain system functionality. (Although this would require commenting out the appropriate code in _real-main.c_ and _game.c_ in order to prevent any issues.)

Everything is controlled through bluetooth commands. The system changes through different states depending on which command is sent. You can see the bluetooth commands in the _communication.c_ library; you can also see the game code for each command in the _game.c_ library. 

Basically, the system goes into a _Connected_ state when bluetooth communication is established. Then it goes to _New Game_ mode, which initializes all the interrupts, the storage, and the player variables. Afterwards it awaits for the _Game Information_ command which tells it the game type, the game limit, the list of enemy numbers, and your own player number. When it receives the _Game Start_ command it enables the receivers and trigger button. From this point on the game is live, and whenever a shot is received it transmits the number and location data to the smartphone; when the trigger is pressed the infrared laser transmits your player code. When the server decides that the game has ended it informs the smartphone, the smartphone then sends a _Game End_ command, which resets all the variables, sends everything that is in storage, and disables the receivers and trigger. 

At this point you could start a new game from the smartphone, and the whole process would be repeated.

---

####Picture of the system

![Picture of Gun](https://raw.github.com/cesarandreu/laser_tag_reloaded/master/Other/Gun_Image.jpg)

This is a picture of the final gun. We purchased this toy gun and modded it to put the board and all the components inside. It connects to a vest through a phone cable. 

---

####Building your own

If you wish to build your own you can find the schematic inside the _Other_ folder. You can also find a list of all the components in our report.

Once you've put the system together you'll have to configure the [command-line toolchain](http://leaflabs.com/docs/unix-toolchain.html). It's only tested on the [maple mini](http://leaflabs.com/docs/hardware/maple-mini.html), but it should work on the maple if you update _initialize.c_ accordingly. 

After that you should be able to clone this repository and run `make`, and then `make install`. 

__WARNING:__ You can take a look at the `Tests` folder for some of the tests we used, or to get an idea of how some of the code works. However, we haven't updated all the files inside, so some may be outdated. Use them at your own risk.

---

####More Information

Inside the _Other_ folder you can find the report that we prepared for our professor. In there you can find information on the components, power analysis, timing analysis, and an in-depth description of the whole system. Pretty much all the information is there. We've included the schematics as well.

There's also a folder with a PCB that we designed. However, we were unable to print and test it. If you decide to print the PCB you'll have to wirewrap two sections in order to ensure uniform ground. We recommend wire-wrapping the ground plane that contains the SDA and SCL pins of the battery monitor to the ground plane containing the Trigger button, and from there, wire-wrap to any other ground-plane in order to ensure uniform ground potential.


If you have any questions feel free to contact us, or just raise an issue. We'll be glad to help if we can. 