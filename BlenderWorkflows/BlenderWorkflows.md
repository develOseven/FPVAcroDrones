# Skeletal Mesh Export

Select the collection with the model and use the `Unreal__whole_model.py` export preset.

Make sure that all the objects that share data (Alt+D linked duplicates) have some differentiator like a modifier, so the UE don't treat them as a single object.

# Static Mesh (for the Conveyor) Export

- temporarily join the parts (using <kbd>Ctrl</kbd>+<kbd>J</kbd>)
- select the collection with the model
- use the `Unreal__whole_model.py` export preset with Armature unchecked

# Character Animation Export

In addition to all the the character actions, have a `_Bind` action with the bind pose on the first and only frame (press <kbd>K</kbd>, choose "Whole Character").
Unreal won't import it as an animation, but it'll use it as the bind pose.

Select the collection with the model and use the `Unreal__whole_model.py` export preset, export only Armature and Other.

# First-person Animation Creation

- animate on the third-person skeleton with the autorotated bones
- use Mwni/blender-animation-retargeting to retarget to the first-person skeleton with the non-autorotated bones
- bake the actions
- in the Scripting tab, run `clavicles_to_1p.py` to move the hands forward
- export character animations

# Icon

<kbd>Ctrl</kbd>+<kbd>F12</kbd> to render an animation to a sequence of png files.

Command to create an icon:

```bash
ffmpeg -framerate 60 -i %4d.png -loop 0 FPVAcroDronesIcon.webp
```
