set term gif animate
set yrange [0:1]
set output 'O.gif'
do for [i=1:59]{
set title sprintf("%d",i)
plot 'outkni_P.txt' u i w l lc 18 t 'kni','outhb_P.txt' u i w l lc 7 t 'hb','outkr_P.txt' u i w l lc 14 t 'kr','outgt_P.txt' u i w l lc 20 t 'gt' 
}




