This is a simple project to manage my floorheating system in the bathroom.
The system is an Uponor Fluvia T Push12 CHX.  It has wireless management of the heating by default and an electric heater that can
be turned on to heat the floor in the summer when the buildings central heatsystem is turned off. 

My native language is swedish and my english is not as good as it ought to be, so some of the wordings are dead wrong. But perhaps in time I will look them up.
The original system uses a wireless control to catch the temperature and control the system. You can select a disired temperature and the
thermostat will open and close accordingly to keep that temperature. And in the summer the system is supposed to switch to summermode; which will close the thermostat 
and kick in the electric heater.  It will not let you manage it from a fancy app, or website or fetch information from google home. And to often the summermode
just dont work for me.  

So I started my own system of managing this.  
I use a raspberry pi3 and a relayboard, and a ds18b20 for measuring temperature.

The way of doing things is the same.  When in wintermode, when the central heatingsystem supplies the radiators with heat, then the 
thermostat open and closes to keep the disired temperature.  And when in summermode, the thermostat is closed and the electric heater kicks in.

As for now, as an initial way of doing things. I just loop a check for disired temperature and actual temperature, and heats for a specific time or does nothing in a
specific time depending on actual temperature.

In time, I will make it cooler. At least thats my hope.  Being a father of two kids; one of is newly born, does not give much time for cool stuff other than cool 
daddystuff.

For questions: (who in their right mind would ask me anything about this? I can hardly think any one else but me can use this,) 
ask me at:  nyberg.kent@gmail.com
