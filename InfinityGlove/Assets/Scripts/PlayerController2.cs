using System.Collections;
using System.Collections.Generic;
using UnityEngine;
public class PlayerController2 : MonoBehaviour
{
    public int speed = 1;
    // public ArduinoConnector x;
    // Use this for initialization
    void Start()
    {
        // x = GameObject.Find("a").GetComponent<PlayerController>().a;
        //a = gameObject.AddComponent<ArduinoConnector>();
        //a.Open();
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
        print("ball 2 collision");
        // x.WriteToArduino("PING2");
        // print(x.ReadFromArduino(1000));
        ArduinoConnector.ping2();
    }
}