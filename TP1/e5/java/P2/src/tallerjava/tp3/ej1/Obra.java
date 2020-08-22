package tallerjava.tp3.ej1;

public abstract class Obra {
	private String titulo;
	private String autor;
	private String dimensiones;
	
	private String getTitulo() {
		return titulo;
	}

	private String getAutor() {
		return autor;
	}

	private String getDimensiones() {
		return dimensiones;
	}

	protected Obra(String titulo, String autor, String dimensiones) {
		this.titulo = titulo;
		this.autor = autor;
		this.dimensiones = dimensiones;
	}

	public String toString() {
		StringBuffer buffer = new StringBuffer();
		buffer.append("Titulo: ").append(this.getTitulo()!=null?this.getTitulo():"- Desconocido -").append("\n");
		buffer.append("Autor: ").append(this.getAutor()!=null?this.getAutor():"- Anonimo -").append("\n");
		buffer.append("Dimensiones: ").append(this.getDimensiones()!=null?this.getDimensiones():"- No especificada -").append("\n");
		
		return buffer.toString();
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((autor == null) ? 0 : autor.hashCode());
		result = prime * result
				+ ((dimensiones == null) ? 0 : dimensiones.hashCode());
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
		Obra other = (Obra) obj;
		if (autor == null) {
			if (other.autor != null)
				return false;
		} else if (!autor.equals(other.autor))
			return false;
		if (dimensiones == null) {
			if (other.dimensiones != null)
				return false;
		} else if (!dimensiones.equals(other.dimensiones))
			return false;
		if (titulo == null) {
			if (other.titulo != null)
				return false;
		} else if (!titulo.equals(other.titulo))
			return false;
		return true;
	}
	
	
}
