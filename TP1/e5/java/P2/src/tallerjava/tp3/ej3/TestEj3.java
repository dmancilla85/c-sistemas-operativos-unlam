package tallerjava.tp3.ej3;

import static org.junit.Assert.*;

import org.junit.Test;

public class TestEj3 {
	
	@Test
	public void testColgadoRecibeLlamada() {	
		Telefono tel=new Telefono();
		tel.atender();
		assertEquals("El estado del telefono debe ser ocupado",new Ocupado().toString(), tel.getEstado().toString());
	}
	
	@Test
	public void testOcupadoRecibeLlamada() {	
		Telefono tel=new Telefono();
		tel.atender();
		tel.atender();
		assertEquals("El estado del telefono debe ser en Espera",new EnEspera().toString(), tel.getEstado().toString());
	}
	
	@Test
	public void testEnEsperaRecibeLlamada() {	
		Telefono tel=new Telefono();
		tel.atender();
		tel.atender();
		tel.atender();
		assertEquals("El estado del telefono debe ser en Espera",new EnEspera().toString(), tel.getEstado().toString());
	}
}
