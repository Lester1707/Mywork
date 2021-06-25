using UnityEngine;
using System.Collections.Generic;
using System;

public class SpawnerController : MonoBehaviour
{
    public GameObject enemy;
    public GameObject flyingEnemy;
    private PlayerControl playerControl;

    public float SpawningSpeed = 1;
    public float GroundY = -3.33f;
    public float FlyHigh = 4f;
    public float GroundCD = 1f;
    public float ObstaclesCD = 5f;
    public float FlyingCD = 2f;

    private float lastGroundSpawn, lastObstaclesSpawn, lastFlyingSpawn;


    void Start() {
        playerControl = FindObjectOfType<PlayerControl>();
        lastGroundSpawn = 0;
        lastObstaclesSpawn = 0;
        lastFlyingSpawn = 0;
    }

    void Update()
    {

        if (UnityEngine.Random.Range((int)0, (int)(10 / (Time.deltaTime * playerControl.speed * SpawningSpeed))) == 0) {
            switch (UnityEngine.Random.Range((int)0, (int)(3))) {
                case 0:
                    //Ground Enemys
                    if(Time.time - lastGroundSpawn > GroundCD) {
                        GameObject new_obj = Instantiate<GameObject>(enemy, new Vector3(transform.position.x,
                                    enemy.GetComponent<BoxCollider2D>().size.y/2*enemy.transform.localScale.y + GroundY,
                                    transform.position.z), new Quaternion(0, 0, 0, 0));
                        lastGroundSpawn = Time.time;
                    }
                    break;
                case 1:
                    //Obstacles
                    if(Time.time - lastObstaclesSpawn > GroundCD) {
                        lastObstaclesSpawn = Time.time;
                    }
                    break;
                case 2:
                    //Flying Enemys
                    FlyHigh = 2 + (float)UnityEngine.Random.Range((int)(-12), (int)(10)) / 10;
                    if(Time.time - lastFlyingSpawn > GroundCD) {
                        GameObject new_obj = Instantiate<GameObject>(flyingEnemy, new Vector3(transform.position.x,
                                    flyingEnemy.GetComponent<BoxCollider2D>().size.y/2*flyingEnemy.transform.localScale.y + FlyHigh,
                                    transform.position.z), new Quaternion(0, 0, 0, 0));
                        lastFlyingSpawn = Time.time;
                    }
                    break;


            }
            
        }

    }
}
