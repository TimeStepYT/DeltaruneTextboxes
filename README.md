# Deltarune Textboxes
![Mod Version](https://api.geode-sdk.org/v1/mods/timestepyt.deltarune_textboxes/status_badge?stat=version)
![Downloads](https://api.geode-sdk.org/v1/mods/timestepyt.deltarune_textboxes/status_badge?stat=downloads)
![GD Version](https://api.geode-sdk.org/v1/mods/timestepyt.deltarune_textboxes/status_badge?stat=gd_version)
![Geode Version](https://api.geode-sdk.org/v1/mods/timestepyt.deltarune_textboxes/status_badge?stat=geode_version)

This Geometry Dash mod turns every Popup into a Deltarune styled Textbox!

You can advance the text by pressing Z, Enter or by clicking/tapping on the screen!
You can skip the text by pressing X or clicking/tapping on the screen!

Keybinds are changeable in the mod's settings!

Apply using the Geode mod loader!

## For mod developers
This mod provides a couple of **events** you can use for better support with your mods!
(For more detailed info, please check Events.hpp or API.hpp if you're using that)

- timestepyt.deltarune_textboxes/create-dialog-box
	- Creates a dialog box with a node you provide where the character sprite would be. (nullptr for no node)
- timestepyt.deltarune_textboxes/create-dialog-box-return
	- The same as before but with an extra parameter to store the created FLAlertLayer in, if you want to modify it yourself.
- timestepyt.deltarune_textboxes/create-dialog-with-voice
	- The same as before but with an extra parameter to set the voice blips
- createFullDialog
	- This event requires you to add DeltaruneTextboxes to your mod's dependencies with `"required": false`
	- You can create full conversations with changing voices and character sprites with it

![[This should show a screenshot]](previews/preview-1.png)

## Sources:
- Fonts:
	- [Determination Extended](https://fontstruct.com/fontstructions/show/2460153/determination-40-7)
	- [Papyrus Pixel](https://online-fonts.com/fonts/papyrus-pixel)
	- [Comic Sans Pixel](https://fontstruct.com/fontstructions/show/1534860/pixel-comic-sans-undertale-sans-font)
- Most assets: Taken from [Undertale](https://store.steampowered.com/app/391540/Undertale/) and [Deltarune](https://store.steampowered.com/app/1671210/DELTARUNE/)