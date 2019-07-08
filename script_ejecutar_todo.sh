
echo "" >> resultados.txt

echo "--------Imagen 720px--------" >> resultados.txt
for hilo in 1 2 3 4 5 6 7 8 9 10;
    do
        for kernel in 4 6 8 10 12 14;
        do
            echo "-------Kernel: $kernel ------ No de Hilos: $hilo ---------" >> resultados.txt
            { time mpirun -np 5 blur-effect 720-image.jpg /home/m-user/outputs/720-image-$kernel-$hilo.jpg $kernel $hilo >/dev/null 2>&1;} |&  tee -a resultados.txt
        done
    done
echo "--------Imagen 1080px--------" >> resultados.txt
for hilo in 1 2 3 4 5 6 7 8 9 10;
    do
        for kernel in 4 6 8 10 12 14;
        do
            echo "-------Kernel: $kernel ------ No de Hilos: $hilo ---------" >> resultados.txt
            { time mpirun -np 5 blur-effect 1080-image.jpeg /home/m-user/outputs/1080-image-$kernel-$hilo.jpeg $kernel $hilo >/dev/null 2>&1;} |&  tee -a resultados.txt
        done
    done
echo "--------Imagen 4K--------" >> resultados.txt
for hilo in 1 2 3 4 5 6 7 8 9 10;
    do
        for kernel in 4 6 8 10 12 14;
        do
            echo "-------Kernel: $kernel ------ No de Hilos: $hilo ---------" >> resultados.txt
            { time mpirun -np 5 blur-effect 4k-image.jpg /home/m-user/outputs/4k-image-$kernel-$hilo.jpg $kernel $hilo >/dev/null 2>&1;} |&  tee -a resultados.txt
        done
    done

