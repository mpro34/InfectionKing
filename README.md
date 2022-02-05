# InfectionKing

## Summary ##
    
Humans have never been created; during the start of the Cretaceous Period 150 Million Years Ago (After the Jurrassic Period), 
a new form of being came into existence, the **Dracoa**. These creatures started as a infectious bacteria that would feed off of the decomposing 
lifeforms throughout the world. They can survive in extreme temperatures and pressures, and would evolve the decomposing body into a new species. 
Fast forward 500 years, the **Dracoa** have become the dominiant species on earth, because of their innnate ability to replicate from the dead bodies all around. 
However, a rare species of **Dracoa** emerged that was able to have coherent thoughts, not just instinct, and could control those **Dracoa** that were lesser. 
This form is known as an Overmind and comprised of 0.001% of the **Dracoa** population.
  
The Overminds rose incredibly quickly to the top of the food chain, using the lesser **Dracoa** to hunt and gather for them. Over the centuries there has been several 
power struggles between the Overminds, each having very unique abilities. Although they have lived in relative peace for years, they still only possess primative 
technology, since they rely on the lesser **Dracoa** to do their bidding. One day, they are put to the test, when an advanced civilization comes from space to colonize their planet. They must figure out how to negotiate, fightback, 
or they face extinction.

You play as various Overminds, in control of a pack of wights. The wights are the lesser **Dracoa** that only use instincts to survive. You are able to control 
the wights and use your inherent bacteria to infect dead organisms to obtain more wights. Since Overminds have strong mental capacities, 
their physical capacities lack. Therefore, they must attack and defend with their pack of weights by shooting enemies with their attack or action pheromone. 
That is, there will be several kinds of pheremones each commanding the wights to perform a specific action. 
The Overminds and only run and dodge from a distance.

Later on, the Overmind gets physical abilties from consuming a wight. This includes the abilities to attack and defend without wights, etc.

## Gameplay ##
- 3rd Person action/shooter
- You start as a newly evolved Overmind who can only melee and shoot pheremones. You have basic actions like running and jumping.
- Enemies will be wild Dracoa or mysterious Overminds.
- The weakest Dracoa (wights), can be consumed using your pheremones to attract them. Pheremones do little damage to wild Dracoa.
- TODO: Mock HUD - Autodesk Sketchbook

## Goal ##
- The player's goal is to get strong enough to defeat the Overmind King, thus becoming the new king.
- In order to get strong enough, gain new abilities from consuming different types of wights.

## Technical Details ##

- Modern C++17
- Custom 3D Game Engine built with OpenGL.
- Building the game engine will also require graphics theory discovery and knowledge gain.
- Use Lua or other Scripting language to easily prototype actual game.

## Progress ##

#### Version 1.0 ####
Version 1.0 of the Dracoa Engine shows rendering of a textured 3d cube with a first-person camera. Also shows the frame stats in the title bar.

![alt text](screenshots/engine_v1.0.jpg)


#### Version 1.1 ####
Version 1.1 adds an OBJ file loading function which no longer uses hard coded vertices.
![alt text](screenshots/engine_v1.1.jpg)

Added 2 parts of the Phong light model: Ambient and Diffuse to a new light bulb model.
![alt text](screenshots/engine_v1.1_diffuse.jpg)


#### Version 1.2 ####
Completed the Phong lighting model by adding Specular lighting to the fragment shader.
![alt text](screenshots/engine_v1.2_specular.jpg)

