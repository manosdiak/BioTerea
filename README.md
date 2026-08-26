# BioTerea
Hardware-in-the-loop simulation and ESP32 control firmware for a biodegradable TEREA stick replacement using a biochar induction susceptor.

An eco-friendly, open-source alternative consumable stick designed to replace non-biodegradable induction-heated tobacco consumables. BioTEREA utilizes an organic biochar and magnetite composite as a biodegradable induction susceptor, eliminating the need for metallic foil inserts.


**Project Overview**

Traditional induction-heated consumables rely on metallic foil blades inside each stick. These non-biodegradable inserts pose significant environmental concerns upon disposal. 

**BioTEREA** replaces metal inserts with a porous, organic carbon matrix (biochar) embedded with magnetic particles (magnetite). When exposed to a high-frequency magnetic field, the magnetite heats via hysteresis while the biochar restricts large eddy currents—creating a tuned, highly resistive induction susceptor. 

Induction Heating: A coil drives an internal coconut-biochar/magnetite ribbon to heat the stick rapidly via magnetic hysteresis.

Vapor Generation: The ribbon heats a low-nicotine tobacco substrate dosed with nicotine benzoate to produce a 350°C aerosol without combustion.

Filtration & Cooling: The vapor passes through an alkaloid-binding tannin trap, a corrugated kraft paper cooling matrix, and a lyocell fiber filter.

Encapsulation: The stick is housed in a candelilla-waxed bagasse tube, sealed with kaolin-reinforced alginate, and wrapped in moisture-resistant paper.

Hardware Integration: The holder detects stick insertion through coil load shifts and uses retuned firmware to manage thermal delivery.

This repository contains the **electromechanical modeling, Python data pipeline, product design and ESP32 control firmware** used to simulate and control the BioTEREA heating process.
