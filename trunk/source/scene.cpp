#include <stdio.h>
#include "scene.h"

Scene::Scene(){
	nxtsc=NULL; //Just in case...
}
Scene::~Scene(){}
void Scene::update(){}
void Scene::start(){}
void Scene::ChangeScene(Scene* nc){
	nxtsc=nc;
}
