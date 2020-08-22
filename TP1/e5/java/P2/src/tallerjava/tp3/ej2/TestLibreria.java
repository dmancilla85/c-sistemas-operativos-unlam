package tallerjava.tp3.ej2;

import static org.junit.Assert.*;

import java.util.Iterator;

import org.junit.Before;
import org.junit.Test;

public class TestLibreria {

	@Before
	public void testLibreriaNoCargada() {
		Libreria l1 = Libreria.getInstance();
		l1.vaciarLibreriaDeAlquileres();
	}
	
	@Test
	public void testAgregarAlguiler() {
		Libreria l1 = Libreria.getInstance();
		LibroDeNovela n1 = new LibroDeNovela();
		
		n1.setTitulo("El Imperio Contrataca");
		n1.setEditorial("Fuerza");
		n1.setAutor("Jona");
		n1.setEdicion("quinta edicion");
		n1.alquilar();
		
		assertTrue("Libreria debe registrar un solo alquiler", l1.getListaAlquiler().size()==1);
		
		Iterator<Alquiler> iteratorAlquileres = l1.getListaAlquiler().iterator();
		
		int contadorAlquilados = 0;
		Libro libro = null;
		
		while (iteratorAlquileres.hasNext()) {
			libro = (Libro) iteratorAlquileres.next().getAlquilable();
			if (libro.getEstaAlquilado() == true) {
				contadorAlquilados++;
			}
		}

		assertTrue("El libro debe estar alquilado", n1.getEstaAlquilado()==true );
		assertTrue("Libreria debe tener un libro sin devolver uno solo", contadorAlquilados==1);
	}
	
	@Test
	public void testAgregarDosAlguileres() {
		Libreria l1 = Libreria.getInstance();
		
		LibroDeEstudio estudio = new LibroDeEstudio();
		estudio.setTitulo("Programando Xiava i Nesta");
		estudio.setEditorial("Capeluz");
		estudio.setTema("Tecnicas buenas de programacion");
		estudio.alquilar();		

		
		LibroDeEstudio novela = new LibroDeEstudio();
		novela.setTitulo("El Cantante Enmascarado");
		novela.setEditorial("Planeta");
		novela.setTema("Vida y Obra");
		novela.alquilar();

		
		novela.devolver();
		
		assertEquals("En Libreria debe haber dos alquilers", 2, l1.getListaAlquiler().size());
		
		int contadorAlquilados = 0, contadorDevueltos = 0;
		Libro libro = null;
		Iterator<Alquiler> iteratorAlquileres = l1.getListaAlquiler().iterator();
		
		while (iteratorAlquileres.hasNext()) {
			libro = (Libro) iteratorAlquileres.next().getAlquilable();
			if (libro.getEstaAlquilado() == true) {
				contadorAlquilados++;
			} else {
				contadorDevueltos++;
			}
		}
		
		assertTrue("El libro de estudio debe estar alquilado", estudio.getEstaAlquilado()==true );
		assertTrue("El libro novela debe estar devuelto", novela.getEstaAlquilado()==false );
		
		assertTrue("Libreria debe tener un libro sin devolver uno solo", contadorAlquilados==1);
		assertTrue("Libreria debe tener un libro devuelto uno solo", contadorDevueltos==1);
		
	}
}