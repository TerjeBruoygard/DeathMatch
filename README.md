[h1]Death Match[/h1]

[h2]Key features[/h2]
[list]
[*]Disabled all diseases and stats except health.
[*]Fast respawn after death.
[*]When killing players, you earn a level and money.
[*]Virtual store with weapons and clothing.
[*]The PVP area is limited by gas. Players outside of it will take damage.
[*]The PVP area expands depending on the number of players on the server.
[*]The PVP area moves around the map.
[/list]

[h2]For server owners[/h2]
[list]
[*]Install the mod. 
[*]Copy the mission folder from the "mpmissions" folder in the mod to the server's "mpmissions" folder.
[*]In the server config "serverDZ.cfg" change/add:
[code]
serverTime="2022/9/17/12/0";
serverTimeAcceleration=0;
serverNightTimeAcceleration=0;
serverTimePersistent=0;
enableCfgGameplayFile = 1;

class Missions
{
    class DayZ
    {
        template="deathMatch.chernarusplus";
    };
};
[/code]
[*]Run server to create default config files in "profiles/DM" folder.
[/list]

[b]Settings.json[/b] - general settings
[code]
- m_trails: list of locations (paths) along which the PVP area will move.Random path will be selected on server starting.
    - m_points: a list of points along which the PVP area will move. After reaching the last point in the list, the PVP area will start moving towards the first point. Random point will be selected on server starting.
    - m_minRadius: minimum (starting) radius of the PVP area.
    - m_maxRadius: maximum (when count of players too much) radius of the PVP area.

- m_expandStep: how many meters the PVP area will expand when a new player connects to the server.
- m_healthRegen: how many health will be regenerated for player in one second.
- m_healthOutOfZoneDmg: modifier how many damage will be received when player out of PVP area. damage = m_healthOutOfZoneDmg * distance_from_area
- m_healthRegenTimeoutOnHit: timeout between last damage and health regen.
- m_startMoney: starting money for player.
- m_moneyPerKill: how much money the player will get for killing.
- m_expIncPerKill: how much expirience the player will get for killing player (murder_level * m_expIncPerKill)
- m_expDecPerDeath: how much experience the player will lose when die (m_expIncPerKill / killer_level)
- m_areaMoveSpeed: how many meters will the PVP area move in 1 second.
[/code]

[b]Weapons.json[/b] - weapons to sell
[code]
- m_Id - short and unique name of weapon preset (used in database, should be unique and short)
- m_Name - name of the presset. Will be displayed in game.
- m_Classname - classname of the weapon.
- m_Attachments - list of attachments for the weapon presset.
- m_Magazine - magazine or ammo to be loaded in weapon when player spawned with this presset.
- m_Level - level to unlock this weapon presset (shold be greater then 0)
- m_Price - price of the weapon presset. Used for first time purchase. Use 0 for free weapon pressets.
[/code]

[b]Equipment.json[/b] - equipment to sell
[code]
- m_Id - short and unique name of equipment preset (used in database, should be unique and short)
- m_Name - name of the presset. Will be displayed in game.
- m_Items - list of items that will be spawned on the player.
- m_Level - level to unlock this weapon presset (shold be greater then 0)
- m_Price - price of the weapon presset. Used for first time purchase. Use 0 for free weapon pressets.
[/code]

[hr][/hr]
[h3]Repacking, modifying or publishing of this mod is prohibited![/h3]

[h3]Author: Terje Bruoygard[/h3]