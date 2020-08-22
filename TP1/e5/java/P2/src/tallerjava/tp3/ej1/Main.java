package tallerjava.tp3.ej1;

public class Main {

	public static void main(String[] args) {
		
		Catalogo c1 = new Catalogo();
		
		for (int i = 0; i < 5; i++) {
			c1.add(new Pintura("LaGioconda","DaVinci","10x15cm"));
			c1.add(new Escultura("El David","Miguel Angel","10x15cm"));
			c1.add(new Grabado("David","Rafael","10x15cm"));
		}
		
		for (Obra obra : c1.getListaObras()) {
			System.out.println(obra.toString());
		}
		

	}

}
