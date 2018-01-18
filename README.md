Name:Joey Bonitati
Clemson Email Address:jbonita@g.clemson.edu
Course number: 4160

Project no:6
Project due date: 12/3

Project description:
This is Space Jam, an epic, realistic, action-packed game which can fulfill your dream of becoming a planet! For years, all of the planets in our Solar system have lived in harmony, but now they decide to fight to the death in a planetary battle royale! Only one planet can be the very best. Will it be you?
You get to play as the most popular planet, Earth. Your movement is controlled by the WASD keys. As you fly around space, clashing with the other planets, moons will start appearing on the battlefield. Pick these up, and they become ammunition which will orbit you until you release them one-by-one with the "Space" bar. If you get hit by too many rogue moons, you will explode, but worry not! You can restart the game by pressing 'R' in order to get another chance at becoming the best planet in the Solar system. If you are the last planet standing, you win!
The other planets are controlled by AI. They actively collect available moons, dodge incoming moons, and shoot the moons at nearby planets.
An informational HUD can be toggled with F1, and another HUD demonstrating proper object pooling of projectiles and explosions can be toggled with F2.

Music: "Space Jam" by Quad City DJ's

Extra Contribution:
Change the <numPlayers> attribute in the XML to unlock Two-Player mode!!!! (wow!)  The second player plays as Venus (Earth's sexy companion) and uses the Arrow Keys to move and Enter to shoot. The two players can take on the enemy planets together and aim for a high score, or they can face off against each other instead!
All sprites are computer-drawn by the author.
Movement is complex! Your character accelerates when you move it, and it slows down due to friction. Acceleration, max speed, and friction as well as other game constants are all read from the game XML, making this program data-driven.
Moons actually orbit your character when you collect them!
Planets interact with each other and the walls at the edge of space to ensure that they do not clip through each other or go off-screen.
Multiple backgrounds move at different speeds to give the illusion of depth.
Painter's algorithm is used to draw spectator suns in the background, which creates an even deeper illusion of depth.
Health bars appear above each planet to show relatively how many more hits they can take.
Each of the planets has unique stats which can also be changed in the XML.
God Mode can be activated by changing the XML constant <godMode> (all the way at the bottom) to the number 17. When god mode is activated, the human-controlled character(s) will collect moons rather than take damage from them.
Sound effects are used to simulate actual explosions as you would hear them in space, making this game so realistic!
Pizzazz!

Problems with this project:
It took too long to make
