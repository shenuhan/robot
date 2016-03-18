package remote;

import static spark.Spark.*;

import java.util.HashMap;
import java.util.Map;

import org.opencv.core.Core;

import spark.template.velocity.VelocityTemplateEngine;

public class Main {

	public static void main(String[] args) {
		System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		
		new Main().init();
	}

	private void init() {
		webSocket("/chat", WebSocketHandler.class);
		staticFileLocation("/public");
		for(String key : new String[]{"up","down","left","right","A","B"} ) {
			get("/" + key, (req, res) -> keyPress(key));		
		}
		get("/joystick/:x/:y", (req, res) -> joystick(req.params(":x"), req.params(":y")));
		get("/", (req, res) -> {
            Map<String, Object> model = new HashMap<>();
            model.put("hello", "Velocity World");
            model.put("person", new Object());

            // The wm files are located under the resources directory
            return new spark.ModelAndView(model, "index.html");
		}, new VelocityTemplateEngine());
		
		new CameraCapture().startCamera();
	}

	private Object keyPress(String key) {
		System.out.println(key);
		return key;
	}

	private Object joystick(String x, String y) {
		System.out.println(String.format("Joystick [%s,%s]", x, y));
		return "joystick";
	}
}
