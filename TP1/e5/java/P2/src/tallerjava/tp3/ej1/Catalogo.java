package tallerjava.tp3.ej1;

import java.util.LinkedList;
import java.util.List;

public class Catalogo {
	private List<Obra> listaObras;
	
	public List<Obra> getListaObras() {
		return listaObras;
	}
	
	public Catalogo() {
		this.listaObras = new LinkedList<Obra>();
	}
	
	public void add(Obra obra) {
		this.listaObras.add(obra);
	}

	
}
