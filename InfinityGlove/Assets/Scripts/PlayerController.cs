using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class PlayerController : MonoBehaviour
{
	public int speed = 1;
    public ArduinoConnector a;
    // Use this for initialization
    void Start()
	{
        a = gameObject.AddComponent<ArduinoConnector>();
        a.Open();
    }
    // Update is called once per frame
    void Update()
	{
		// get input data from keyboard or controller
		float moveHorizontal = Input.GetAxis("Horizontal");
		float moveVertical = Input.GetAxis("Vertical");
		// update player position based on input
		Vector3 pos = transform.position;
		pos.x += moveHorizontal * speed * Time.deltaTime;
		pos.z += moveVertical * speed * Time.deltaTime;
		transform.position = pos;
    }
    private void OnMouseDown()
    {
        print("ball 1 collision");
        // a.WriteToArduino("PING1");
        // print(a.ReadFromArduino(1000));
        ArduinoConnector.ping1();
    }
}