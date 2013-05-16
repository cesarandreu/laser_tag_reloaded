#Laser Tag Reloaded

---

##What is this

This is a laser tag system which connects to your android smartphone using bluetooth. Your smartphone then connects to a remote server which handles tracking the lobby and game data. 

The program flow is simple: You pair the smartphone with the gun/vest, you join or create a lobby, and then you play! 

Each player is assigned a number, which gets transmitted by the infrared laser. When another player receives a valid enemy number it transmits this to the smartphone, and then the smartphone transmits it to the server.

At the end of each game you can see a map with markers showing the places where you got shot. 

---

###Project Links

The project itself has three components: 

* [__System__ (Gun + Vest)](https://github.com/cesarandreu/laser_tag_reloaded)

* [__Server__ (Node.js) ](https://github.com/cesarandreu/laser_tag_nodejs)

* [ __Smartphone application__ (Android)](https://github.com/cesarandreu/laser_tag_android)

---

###System

The system (gun + vest) has an infrared laser, two infrared receivers, a speaker, a bluetooth module, a GPS module, a battery gauge, a battery charger, a battery, an LED bar graph, and two push-buttons. 

The essential components are: the infrared laser, the infrared receivers, the speaker, the bluetooth, and the push-button. (And a power source.) These components are required for the system to function. You could remove the LED graph, second button, battery charger, battery gauge, and GPS, and still maintain system functionality. (Although this would require commenting out the appropriate code in _real-main.c_ and _game.c_ in order to prevent any type of issue.)

Everything is controlled through bluetooth commands. The system changes through different states depending on which command is sent. You can see the bluetooth commands in the _communication.c_ library; you can also see the game code for each command in the _game.c_ library. 

Basically, the system goes into a _Connected_ state when bluetooth communication is established. Then it goes to _New Game_ mode, which initializes all the interrupts, the storage, and the player variables. Afterwards it awaits for the _Game Information_ command which tells it the game type, the game limit, the list of enemy numbers, and your own player number. When it receives the _Game Start_ command it enables the receivers and trigger button. From this point on the game is live, and whenever a shot is received it transmits the number and location data to the smartphone; when the trigger is pressed the infrared laser transmits your player code. When the server decides that the game has ended it informs the smartphone, the smartphone then sends a _Game End_ command, which resets all the variables, sends everything that is in storage, and disables the receivers and trigger. 

If the players wished to, they could start a new game from the smartphone; at which point the whole process would be repeated.

---

####Building your own

If you wish to build your own you can find the schematic inside the _Other_ folder.

Once you've put the system together you'll have to configure the [command-line toolchain](http://leaflabs.com/docs/unix-toolchain.html).

After that you should simply be able to clone this repository and be able to run `make` and then `make install`. 

---

####More Information

Inside the _Other_ folder you can find the report that we prepared. In there you can find information on the components, power analysis, timing analysis, and an in-depth description of the whole system. 

If you have any questions feel free to contact us, or just raise an issue. We'll be glad to help if we can. 