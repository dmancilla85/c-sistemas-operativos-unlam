package tallerjava.tp3.ej2;

public class Libro {

	private String editorial;
	private String titulo;
	private boolean estaAlquilado;
	
	public String getEditorial() {
		return editorial;
	}
	public void setEditorial(String editorial) {
		this.editorial = editorial;
	}
	public String getTitulo() {
		return titulo;
	}
	public void setTitulo(String titulo) {
		this.titulo = titulo;
	}
	public boolean getEstaAlquilado() {
		return estaAlquilado;
	}
	public void setEstaAlquilado(boolean estaAlquilado) {
		this.estaAlquilado = estaAlquilado;
	}
	
	public Libro() {
		this("No determinado","No determinado",false);
	}
	
	public Libro(String editorial, String titulo, boolean estado) {
		this.editorial = editorial;
		this.titulo = editorial;
		this.estaAlquilado = estado;
	}
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((editorial == null) ? 0 : editorial.hashCode());
		result = prime * result + (estaAlquilado ? 1231 : 1237);
		result = prime * result + ((titulo == null) ? 0 : titulo.hashCode());
		return result;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Libro other = (Libro) obj;
		if (editorial == null) {
			if (other.editorial != null)
				return false;
		} else if (!editorial.equals(other.editorial))
			return false;
		if (estaAlquilado != other.estaAlquilado)
			return false;
		if (titulo == null) {
			if (other.titulo != null)
				return false;
		} else if (!titulo.equals(other.titulo))
			return false;
		return true;
	}
	
	
}
