package vyglab.argeo.test;

import vyglab.argeo.jni.BillboardGraphics;
import vyglab.argeo.jni.Entity;
import vyglab.argeo.jni.EntityCollection;
import vyglab.argeo.jni.Geodetic3D;
import vyglab.argeo.jni.enums.HeightClamping;
import vyglab.argeo.jni.ModelGraphics;
import vyglab.argeo.jni.enums.OrientationClamping;

public class EntityTest {

	public static void addEntities(EntityCollection collection){

		Entity uns = new Entity(
				"UNS",
				new BillboardGraphics(
						"icons/032.png",
						"university",
						50,
						50,
						HeightClamping.GROUND));

		Entity buffet = new Entity(
				"Buffet",
				new ModelGraphics(
						"models/chateau/chateau.obj",
						HeightClamping.GROUND,
						OrientationClamping.TRUE_NORTH));


		buffet.setPosition(new Geodetic3D(
				-38.67713,
				-62.23888));

        uns.setPosition(new Geodetic3D(
                -38.67713,
                -62.23888));

		/*
		Entity ventana = new Entity(
				"Ventana",
				new PlaneGraphics(new EllipsoidTangentPlane(new Geodetic3D(
						-38.118629,
						-61.959423))));



		buffet.setOrientation(
				Quaternion.fromYawPitchRoll(
						0.40,
						0.74,
						0.80));

		Entity chulengo = new Entity(
				"Chulengo",
				new ModelGraphics(
						"models/Mark 42/Mark 42.obj",
						HeightClamping.GROUND,
						OrientationClamping.TRUE_NORTH));

		chulengo.setOrientation(
				Quaternion.fromYawPitchRoll(
						0.40,
						0.74,
						0.80));

		Entity casa = new Entity(
				"ch",
				"Chulengo",
				new ModelGraphics(
						"models/Mark 42/Mark 42.obj",
						HeightClamping.GROUND,
						OrientationClamping.TRUE_NORTH));

		casa.setOrientation(
				Quaternion.fromYawPitchRoll(
						0.40,
						0.74,
						0.80));

		casa.setPosition(new Geodetic3D(
				-38.724524,
				-62.260409));

		roca.setPosition(new Geodetic3D(
				-38.715719,
				-62.271632));

		militares.setPosition(new Geodetic3D(
				-38.681999,
				-62.260170));

		tres_picos.setPosition(new Geodetic3D(
				-38.118629,
				-61.959423));

		uns.setPosition(new Geodetic3D(
				-38.701473,
				-62.270249));

		ventana.setPosition(new Geodetic3D(
				-38.0486,
				-62.0189));

		chulengo.setPosition(new Geodetic3D(
				-38.695094,
				-62.249522));

		*/


		//collection.add(uns);
		//collection.add(militares);
        //collection.add(buffet);

		/*
		collection.add(chulengo);

		collection.add(casa);
		collection.add(ventana);
		*/
	}
}
