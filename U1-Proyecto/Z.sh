NombreProgramaFuente=ProyectoU1

rm $NombreProgramaFuente
g++ $NombreProgramaFuente.cpp -o $NombreProgramaFuente -lGL -lglut -lGLEW -lm
./$NombreProgramaFuente
