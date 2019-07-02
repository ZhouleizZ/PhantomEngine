#include "SceneManager.h"
#include <iostream>
#include "AssetLoadManager.h"
#include "opengexparser.h"
using namespace std;
using namespace Phantom;

int Phantom::SceneManager::Init()
{
	int result = 0;
	m_pScene = make_shared<Scene>("init scene");
	LoadScene();
	return result;
}

void Phantom::SceneManager::Shutdown()
{
	cout << "Shutdown" << endl;
}

void Phantom::SceneManager::Tick()
{
#ifdef DEBUG
	cout << "Tick" << endl;
#endif
#ifdef _DEBUG
	//cout << "_Tick" << endl;
#endif
}

const Scene & SceneManager::GetSceneForRendering()
{
	return *m_pScene;
}

bool Phantom::SceneManager::LoadScene()
{
	string ogex_text = g_pAssetLoader->SyncOpenAndReadTextFileToString("Resources/Scene/anim.ogex");
	//string ogex_text = g_pAssetLoader->SyncOpenAndReadTextFileToString("Resources/Scene/my.ogex");
	if (ogex_text.empty()) {
		return false;
	}
	OpengexParser paser;
	m_pScene = paser.Parse( ogex_text);
	m_pScene->camera->initViewMatrix();
	if (!m_pScene) {
		return false;
	}
	m_pScene->LoadTextures();
	return true;
}
