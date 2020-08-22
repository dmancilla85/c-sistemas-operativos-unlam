package tallerjava.tp3.ej2;

import java.util.Date;

public abstract class LibroAlquilable extends Libro implements Alquilable {

	@Override
	public void alquilar() {
		Libreria l1 = Libreria.getInstance();
		
		Alquiler alquiler = new  Alquiler();
		alquiler.setAlquilable(this);
		this.setEstaAlquilado(true);
		l1.agregarAlquiler(alquiler);
	}
	
	@Override
	public void devolver() {
		Libreria l1 = Libreria.getInstance();
		
		try 
		{
			Alquiler alquiler = l1.buscarAlquiler(this);
			alquiler.setFechaDevolucion(new Date());
			this.setEstaAlquilado(false);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
	}
}
