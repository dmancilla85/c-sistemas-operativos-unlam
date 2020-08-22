package tallerjava.tp3.ej2;

import java.util.LinkedList;
import java.util.List;

public class Libreria {
	private static Libreria instance;
	private List<Alquiler> listaAlquiler;

	private Libreria() {
		listaAlquiler = new LinkedList<Alquiler>(); // Usamos LinkedList porque  no sabemos cuanto va a crecer la lista de alquileres.
	}

	public static Libreria getInstance() {
		if (instance == null)
			instance = new Libreria();
		return instance;
	}

	public List<Alquiler> getListaAlquiler() {
		return listaAlquiler;
	}
	
	public void agregarAlquiler(Alquiler alquiler) {
		if(alquiler != null)
			this.listaAlquiler.add(alquiler);
		else
			System.out.println("No se puede agregar el libro a la lista de alquiler porque no se registro el alquiler");
	}

	public void listarAlquileres() {
		Libro libro = null;
		StringBuilder builder = new StringBuilder();
		for (Alquiler alquiler : listaAlquiler) {
			libro = (Libro) alquiler.getAlquilable();
			if (libro.getEstaAlquilado()==true) {
				builder.append("Titulo: ").append(libro.getTitulo());
				builder.append("\tFecha de Alquiler: ").append(alquiler.getFechaAlquiler());
				System.out.println(builder.toString());
			}
		}
	}
	
	public Alquiler buscarAlquiler(Libro libroBuscar) throws Exception{
		Libro libro = null;
		for (Alquiler alquiler : listaAlquiler) {
			libro = (Libro)alquiler.getAlquilable();
			if(libro.equals(libroBuscar)==true)
				return alquiler;
		}
		throw new Exception();
	}
	
	public void vaciarLibreriaDeAlquileres(){
		this.listaAlquiler.clear();
	}
}
