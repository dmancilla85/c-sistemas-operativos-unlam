package tallerjava.tp3.ej1;

import static org.junit.Assert.*;

import java.util.LinkedList;
import java.util.List;

import org.junit.Test;

public class TestCatalogo {

	@Test
	public void testCatalogo() {
		
		Catalogo catalogo = new Catalogo();
		List<Obra> lista = new LinkedList<Obra>();
		
		for (int i = 0; i < 5; i++) {
			catalogo.add(new Pintura("LaGioconda "+i,"DaVinci","10x15cm"));
			lista.add(new Pintura("LaGioconda "+i,"DaVinci","10x15cm"));
			
			catalogo.add(new Escultura("El David "+i,"Miguel Angel","10x15cm"));
			lista.add(new Escultura("El David "+i,"Miguel Angel","10x15cm"));
			
			catalogo.add(new Grabado("La escuela de Atenas "+i,"Rafael","10x15cm"));
			lista.add(new Grabado("La escuela de Atenas "+i,"Rafael","10x15cm"));
		}
		
		for (int i = 0; i < catalogo.getListaObras().size(); i++) {
			assertTrue(catalogo.getListaObras().get(i).equals(lista.get(i)));
		}
	}


}
