set datafile separator ','
set terminal png size 1333,1000
set output "averages-histogram.png"
set style data histograms
set style fill solid
plot 'average.csv' using 2:xtic(1) title 'kernel', '' u 3:xtic(1) t 'initrd', '' u 4:xtic(1) t 'userspace', '' u 5:xtic(1) t 'total_boot_time', '' u 6:xtic(1) t 'initrd_switch_root.service', '' u 7:xtic(1) t 'NetworkManager-wait-online.service', '' u 8:xtic(1) t 'NetworkManager.service', '' u 9:xtic(1) t 'modprobe@drm.service', '' u 10:xtic(1) t 'systemd-logind.service'
