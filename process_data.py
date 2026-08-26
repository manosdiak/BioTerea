import numpy as np

filename = 'susceptor_current_sweep.txt'

# Parse LTspice stepped output file manually
steel_time, steel_current = [], []
biochar_time, biochar_current = [], []

current_step = 0

with open(filename, 'r') as f:
    for line in f:
        # Check for step headers or skip table column header
        if 'Step' in line or 'time' in line.lower() or 'i(' in line.lower():
            if 'step' in line.lower():
                current_step += 1
            continue
        
        parts = line.strip().split()
        if len(parts) >= 2:
            try:
                t = float(parts[0])
                i = float(parts[1])
                
                if current_step <= 1:
                    steel_time.append(t)
                    steel_current.append(i)
                else:
                    biochar_time.append(t)
                    biochar_current.append(i)
            except ValueError:
                continue

# Convert to numpy arrays
i_steel = np.array(steel_current)
i_biochar = np.array(biochar_current)

# If step headers were missing, split dataset in half symmetrically
if len(biochar_current) == 0:
    mid = len(i_steel) // 2
    i_biochar = i_steel[mid:]
    i_steel = i_steel[:mid]

# Windowed RMS calculation function
def extract_rms(signal, window=500, step=250):
    rms_vals = []
    for i in range(0, len(signal) - window, step):
        segment = signal[i:i + window]
        rms_vals.append(np.sqrt(np.mean(segment**2)))
    return np.array(rms_vals)

rms_steel = extract_rms(i_steel)
rms_biochar = extract_rms(i_biochar)

# Downsample to 100 data points for MCU memory footprint
target_pts = 100
idx_s = np.linspace(0, len(rms_steel) - 1, target_pts, dtype=int)
idx_b = np.linspace(0, len(rms_biochar) - 1, target_pts, dtype=int)

steel_pts = rms_steel[idx_s]
biochar_pts = rms_biochar[idx_b]

# Write C-Header File for PlatformIO
with open('simulated_load.h', 'w') as f:
    f.write('#ifndef SIMULATED_LOAD_H\n#define SIMULATED_LOAD_H\n\n')
    f.write('#include <stdint.h>\n\n')
    f.write(f'#define LOAD_DATA_POINTS {target_pts}\n\n')
    
    f.write('// Inferred RMS Load Profiles (Amperes)\n')
    f.write('const float RMS_STEEL_BASELINE[LOAD_DATA_POINTS] = {\n    ')
    f.write(', '.join([f'{v:.3f}f' for v in steel_pts]))
    f.write('\n};\n\n')
    
    f.write('const float RMS_BIOCHAR_COMPOSITE[LOAD_DATA_POINTS] = {\n    ')
    f.write(', '.join([f'{v:.3f}f' for v in biochar_pts]))
    f.write('\n};\n\n#endif\n')

print("simulated_load.h successfully generated!")