<h3 align="center">ReGameDLL_CS Spread Control Plugin</h3>

<p align="center">Control the FireBullets3 function vecSpread parameter</p>

<p align="center">
    <a href="https://github.com/SmileYzn/spread/issues"><img alt="GitHub Issues" src="https://img.shields.io/github/issues-raw/smileyzn/spread?style=flat-square"></a>
    <a href="https://github.com/SmileYzn/spread/actions"><img alt="GitHub Workflow Status" src="https://img.shields.io/github/actions/workflow/status/SmileYzn/spread/build.yml?branch=main&label=Build&style=flat-square"></a>
</p>

<hr>

<p align="center">This plugin control the vecSpread variable of FireBullets3 on <a href="https://github.com/rehlds/ReGameDLL_CS/blob/master/regamedll/dlls/cbase.cpp#L1268" target="_blank">cbase.cpp</a></p>
<p align="center">The plugin remove bullet spread on these situations:</p>
<ol>
  <li>Player is on ground.</li>
  <li>Weapon Recoil (Player->pev->punchangle) is zero (The player AIM is centered).</li>
  <li>With the player walking slower than half the maximum speed of the weapon they are carrying</li>
</ol>

<br>

<p align="center">The plugin removed spread from first bullet shots.</p>
<p align="center">It does not affect recoil, nor does it change the default behavior of aim, it just adjusts the insane pattern of shots in the game.</p>
<p align="center">You can use this plugin as replacement for my <a href="https://github.com/smileYzn/accuracyfix" target="_blank">Accuracy Fix Plugin</a>.</p>

<hr>

<details>
  <summary align="center">Spread Control Settings (Click to expand)</summary><br>
    
```
// Activate Spread Control Plugin
//
// 0 Disable
// 1 Enable
//
// Default "1"
sc_active "1"
```
</details>
