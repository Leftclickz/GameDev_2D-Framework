Crypt of the Necrodancer Ripoff - Version 0.01337 by Dayton Heywood

Basic controls
WASD to move. Hitting an enemy will damage them. Enemies will always move after the player's input.

You must move on the beat of the music or risk losing input and having the enemies get a free move against you.

Killing enemies increases your score.

When you kill all enemies on a floor, you will move on to the next floor (there's 5 floors total, but they're extremely low effort, don't
look into them for anything more than just proof of concept)

The save file includes health, score, and the current floor you are on

KNOWN ISSUES:

	- Input is sometimes janky, I did my best to get it to work perfectly on beats but sometimes it wigs out a bit.
	- Health and dust particles do not display properly. I was intending to use instanced draw calls to take care of health bars
	(I have 2 versions of the health sprite for player and enemies) but I couldn't figure out how to get instance draw calls working.
	I was VERY CLOSE! If you look at the bottom right of the screen when you boot up you'll see the Dust animation atlas drawing in a corner
	when you move instead of where it should be.

What to actually be proud of:

	- Very basic sound organization
	- Very basic image loading
	- Debugging. I tried to keep my code dynamic enough to allow for different draw calls to see lines of my meshes. That helped a LOT
	for figuring out logic issues in my program (ESPECIALLY THE CANVAS)
	- AStar implemented and works dynamically with each level
	- Adjustable text boxes with basic alignment
	- My Canvas class. I put way more effort into that than I should have. Budgetting time should have been more important to me but
	the final result turned out very well I think.

Final thoughts:

I wanted to get a lot more done on this. There's so much that can be expanded on and for all intents and purposes the framework of making a game
is in place, but I put a looooot of time into setting it all up that I didn't spend as much as I'd like actually implementing the game itself.
Food for thought next semester, keep it small and work up later.