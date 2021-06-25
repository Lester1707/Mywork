using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    private PlayerControl playerControl;
    public float imSpeed;
    public  float startSpeed;
    public float r;
    private float r0;
    private float k;
    private float x0;
    public float cameraSpeed = 8;
    public float cameraSoftness = 5;

    // Start is called before the first frame update
    void Start()
    {
        playerControl = FindObjectOfType<PlayerControl>();
        imSpeed = playerControl.speed;
        startSpeed = imSpeed;
        //startSpeed = 0;
        x0 = playerControl.transform.position.x;
        r0 = transform.position.x - x0;
        r = transform.position.x - x0;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        r = r - (playerControl.speed - imSpeed) * 0.01f* cameraSoftness;
        imSpeed = startSpeed + (r0 - r) * 0.01f * cameraSpeed;
        transform.position = Vector3.Lerp(transform.position, new Vector3(x0 + r, transform.position.y, transform.position.z), 100 *  Time.deltaTime);
    }
}
