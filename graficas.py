import matplotlib.pyplot as plt
import soundfile as sf
import numpy as np

pitch=[]
ref_pitch = []
pitch = [x.split()[0] for x in open('./rl002.f0').readlines()]
pitch = [float(x) for x in pitch]
ref_pitch = [x for x in open('./rl002.f0ref').readlines()]
ref_pitch = [float(x) for x in ref_pitch]

# senyal_original, fm1 = sf.read('rl002.wav ')
# T0 = int(0.621 * fm1)
# D = int(0.03 * fm1)
t = np.arange(len(pitch))
# senyal = senyal_original[T0:T0+D]

# r = np.zeros(D)


# for k in range(D):
#   for n in range(len(senyal)-k):
#     r[k] = r[k] + (senyal[n] * senyal[k+n])   
#   r[k] = r[k] /len(senyal)

# print(f'Periodo: {fm1/155}')

f, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,pitch,'-')
ax1.set_title('Calculated pitch')
ax1.set_ylabel('Hz')

ax2.plot(t,ref_pitch,'-')
ax2.set_title('Reference pitch')
ax2.set_ylabel('Hz')

plt.show()