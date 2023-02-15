# AlchemyProject

Developed with Unreal Engine 5 using both C++ and Blueprints

Currently the project contains the following systems:

- Inventory system
- Alchemy System
- Dialogue System
- Faction System
- AI

1. Inventory system allows the player or the AI to have a personal inventory that can store any item that implements the correct interface.
Currently the inventory consists of set number of slots where each different item is assigned to. Player can pick up one or multiple of the same items at a time. Items can be inspected, used or dropped. Items are organized into slots based on the hash code that gets generated for each item. This hash code ensures that even if a potion is named the same, but for example the quality is different, the new potion will be assigned to another slot. 
Note: The current inventory system will be reworked so that there is no limit on the number of slots. This will be done using a listview.

2. Alchemy system currently allows the player to pick up alchemy ingredients and brew potions out of them. Each potion has a recipe associated with it. Depending on the quality of the ingredients used, the potion's quality will change. Higher quality potions provide better effects, but the implementation is unique to each potion. Potions consist of the potion actor and a potion component. The potion component will be created and then attached to the user for the duration of the potion. The logic for the potion is written inside the potion component. The logic can be written either in Blueprints or C++. This way of implementing potions makes sure that the character using the potion doesn't need to implement any logic for each different potion.

3. Dialogue system allows the player to converse with the AI. Dialogue is stored inside a JSON format and read by the corresponding AI when needed. Each dialogue can have as many different dialogue options as possible. Dialogue options serve multiple functions. They can trigger events, such as healing the player for a predetermined amount of hitpoints, or be linked together to unlock further dialogue. Some dialogue options can also be hidden and only become visible when certain criteria are met. Additionally, the content of each dialogue option is unique to the NPC it is associated with, and can vary depending on the NPC's ID.

4. The faction system is still in its infancy, but currently the system allows each NPC to be part of a faction. Factions are defined by various attributes, including a membership list and a set of relationships with other factions. This includes designations of hostile, neutral, and friendly factions, and a map of opinions used to determine relations with other factions. AI perception takes factions into consideration, so that it treats members of hostile factions as enemies.

5. The AI in its current form can have simple conversations with the player, can execute work related behavior, like "gathering" alchemy ingredients, chase hostiles, search for hostiles, patrol and move to locations using the road network. All of this is done using behavior trees, dynamic sub-trees, behavior tree tasks, decorators, services and EQS. 
