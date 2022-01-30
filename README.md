# FEUP-AED-Project-2

Given a dataset from the [SCTP](https://www.stcp.pt/pt/viajar/) (Sociedade de Transportes Coletivos do Porto), we provide a user options to calculate routes.

The user can choose a stop to start by its code or provide a location, given the coordinates and a distance, and the stops that are at a maximum distance from the location
will be presented.

The user can choose the starting stop by the following options:
- Provide the code of a stop
- Provide a location, given its coordinates

There are 2 options to find the best route:
- The one with the shortest distance
- The one that goes through the fewest stops

The destination to the trip can be chosen by 2 different methods:
- Provide a location, given its coordinates
- Provide the code of a stop

If the user provides a location, the program will calculate the distance between the closest stop from that location and display it to the user.

In all the possible routes, the stops that are part of it are displayed, from the starting to the ending stop.

Finally, if the user wants to go through all the stops given a starting stop, the program will display the minimum distance needed to do that.

This project was developed by David Fang, João Teixeira and Jorge Sousa.
