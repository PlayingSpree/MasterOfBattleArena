#include "SceneGame.h"
#include "Effect.h"

unsigned int SceneGame::UID = 0;

SceneGame::SceneGame(Game* g) : map(g, this, (MapClass::MapEnum)g->playdata.map, &EnemySpawnQueue)
{
	g->playdata.Score = 0U;
	UID = 0;
	game = g;
	Map = (MapClass::MapEnum)g->playdata.map;

	loadTexture();
	AddBackground();

	float x = -24 * (game->playdata.playerCount-1);
	for (int i = 0; i < game->playdata.playerCount; i++) {
		AddNewGameObject(new Player(game, game->playdata.playerinfo[i].PlayerModel, i, game->playdata.playerinfo[i].controlnum, game->playdata.playerinfo[i].team, Player::BaseStatStruct(100.0f, 5.0f, 100.0f),sf::Vector2f(x,0)));
		x += 48;
	}

	while (!map.MapObject.empty())
	{
		AddNewGameObject(map.MapObject.front());
		map.MapObject.pop();
	}
	g->sound.PlayMusic(SoundEngine::ME_Game_Intro);
	g->sound.Music.setLoop(false);
}

void SceneGame::AddNewGameObject(GameObject* gop) {
	objectvector.push_back(gop);
	ObjectByTypeMapVector[gop->ObjectType].push_back(gop);
}

std::vector<GameObject*>::iterator SceneGame::RemoveGameObject(std::vector<GameObject*>::iterator dit) {

	std::map<GameObject::ObjectTypeEnum, std::vector<GameObject*>>::iterator it = ObjectByTypeMapVector.find((*dit)->ObjectType);
	if (it != ObjectByTypeMapVector.end()) {
		for (int i = 0; i < (*it).second.size(); i++) {
			if (*dit == (*it).second[i])
				(*it).second.erase((*it).second.begin() + i);
		}
	}
	delete (*dit);
	return objectvector.erase(dit);
}

SceneGame::~SceneGame()
{
	while (!objectvector.empty())
	{
		delete objectvector.back();
		objectvector.pop_back();
	}
	if (!backgroundSpriteList.empty())
	{
		for (std::list<sf::Sprite*>::iterator it = backgroundSpriteList.begin(); it != backgroundSpriteList.end(); it++)
		{
			delete *it;
		}
		backgroundSpriteList.clear();
	}
}

void SceneGame::NextWave() {
	unsigned int bonus = 0;
	for (int i = 0; i < ObjectByTypeMapVector[GameObject::PlayerObj].size(); i++) {
		if (((Player*)ObjectByTypeMapVector[GameObject::PlayerObj][i])->Die) {
			((Player*)ObjectByTypeMapVector[GameObject::PlayerObj][i])->Die = false;
			((Player*)ObjectByTypeMapVector[GameObject::PlayerObj][i])->PStatus.HP = 10.0f;
			((Player*)ObjectByTypeMapVector[GameObject::PlayerObj][i])->PStatus.HPRegenCD = 0.0f;
			((Player*)ObjectByTypeMapVector[GameObject::PlayerObj][i])->PStatus.speed = sf::Vector2f();
		}
		else
		{
			game->playdata.Score += 5000U;
			bonus += 5000U;
		}
	}
	(((SceneGame*)(game->scene))->UIFloatingTextList.push_back(SceneGame::FloatingText("Survival Bonus (X" + std::to_string(bonus / 5000U) + ") : +" + std::to_string(bonus), sf::Vector2f(game->window->getSize().x / 2.0f, 220), SceneGame::FloatingText::Zoom, game->GameResource, 40)));
}

void SceneGame::loop()
{
	if (game->sound.Music.getStatus() == sf::Sound::Stopped) {
		if(game->sound.Mplaying == SoundEngine::ME_Game_Intro)
			game->sound.PlayMusic(SoundEngine::ME_Game_wave1Intro);
		else if (wave < 5)
			game->sound.PlayMusic(SoundEngine::ME_Game_wave1);
		else if (wave < 10)
			game->sound.PlayMusic(SoundEngine::ME_Game_wave5);
		else
			game->sound.PlayMusic(SoundEngine::ME_Game_wave10);
	}
	game->window->clear(sf::Color(0, 0, 0, 255));
	//Timer
	EnemySpawnQueueTime -= game->deltatime;
	//SpawnEnemy
	if (!wavecompleted) {
		if (ObjectByTypeMapVector[GameObject::EnemyObj].size() == 0) {
			game->playdata.Score += (unsigned int)(EnemySpawnQueueTime * 200.0f);
			if (EnemySpawnQueueTime > 0)
				(((SceneGame*)(game->scene))->UIFloatingTextList.push_back(SceneGame::FloatingText("Time Bonus (" + std::to_string((int)EnemySpawnQueueTime) + "s) : +" + std::to_string((unsigned int)(EnemySpawnQueueTime * 200.0f)), sf::Vector2f(game->window->getSize().x / 2.0f, 170), SceneGame::FloatingText::Zoom, game->GameResource, 40)));
			if (EnemySpawnQueueTime > 10.0f) {
				EnemySpawnQueueTime = 10.0f;
			}
			(((SceneGame*)(game->scene))->UIFloatingTextList.push_back(SceneGame::FloatingText("Wave Complete!", sf::Vector2f(game->window->getSize().x / 2.0f, 100), SceneGame::FloatingText::Zoom, game->GameResource, 80)));
			wavecompleted = true;
			NextWave();
		}
	}
	if (EnemySpawnQueueTime <= 0) {
		if (EnemySpawnQueue.empty()) {
			map.SpawnWave(++wave, true, ObjectByTypeMapVector[GameObject::PlayerObj].size());
			if (wave == 5)
				game->sound.PlayMusic(SoundEngine::ME_Game_wave5Intro);
			if (wave == 10)
				game->sound.PlayMusic(SoundEngine::ME_Game_wave10Intro);
			if (wave != 1 && !wavecompleted)
				NextWave();
			(((SceneGame*)(game->scene))->UIFloatingTextList.push_back(SceneGame::FloatingText("Wave " + std::to_string(wave), sf::Vector2f(game->window->getSize().x / 2.0f, 300), SceneGame::FloatingText::Zoom, game->GameResource, 100)));
		}
		std::vector<Spawner*> sv;
		sv.push_back((Spawner*)ObjectByTypeMapVector[GameObject::SpawnerObj][0]);
		for (int i = 1; i < ObjectByTypeMapVector[GameObject::SpawnerObj].size(); i++) {
			bool inserted = false;
			for (int i2 = 0; i2 < sv.size(); i2++) {
				if (GGEZ::LengthVector2f(ObjectByTypeMapVector[GameObject::SpawnerObj][i]->GetPosition() - game->window->getView().getCenter()) > GGEZ::LengthVector2f(sv[i2]->GetPosition() - game->window->getView().getCenter())) {
					sv.insert(sv.begin() + i, (Spawner*)ObjectByTypeMapVector[GameObject::SpawnerObj][i]);
					inserted = true;
					break;
				}
			}
			if (!inserted) {
				sv.push_back((Spawner*)ObjectByTypeMapVector[GameObject::SpawnerObj][i]);
			}
		}
		std::vector<Spawner*>ssv(sv);
		while (EnemySpawnQueueTime <= 0)
		{
			Enemy* e = new Enemy(game, -1, EnemySpawnQueue.front().Enemy);
			int spawnpos = rand() % (int)fmin(4, ssv.size());
			e->SetPosition(ssv[spawnpos]->GetPosition());
			if (Map == MapClass::Map2)
				AddNewGameObject(((GameObject*)(new Effect(game, ssv[spawnpos]->GetPosition().x, ssv[spawnpos]->GetPosition().y, Effect::Map2Spawnpoint, 1))));
			ssv.erase(ssv.begin() + spawnpos);
			AddNewGameObject(e);
			EnemySpawnQueueTime = EnemySpawnQueue.front().WaitTime;
			EnemySpawnQueue.pop();
			if (ssv.empty()) {
				ssv = sv;
			}
		}
		if (EnemySpawnQueue.empty()) {
			EnemySpawnQueueTime = 30.0f;
			wavecompleted = false;
		}
	}
	//Create Object
	for (std::list<GameObject*>::iterator it = newobjectlist.begin(); it != newobjectlist.end();)
	{
		AddNewGameObject(*it);
		it = newobjectlist.erase(it);
	}
	//Update Object
	for (std::vector<GameObject*>::iterator it = objectvector.begin(); it != objectvector.end();)
	{
		if ((*it)->Update(game->deltatime)) {
			it = RemoveGameObject(it);
		}
		else
		{
			it++;
		}
	}
	int die = 0;
	for (int i = 0; i < ObjectByTypeMapVector[GameObject::PlayerObj].size(); i++) {
		if (((Player*)ObjectByTypeMapVector[GameObject::PlayerObj][i])->Die) {
			die++;
		}
	}
	if (ObjectByTypeMapVector[GameObject::PlayerObj].size() == die) {
		game->ChangeScene(Game::SMainMenu);
		return;
	}
	//UpdateView
	UpdateView();
	//Draw Background
	if (Map == MapClass::Map2)
	{
		float factor;
		unsigned int width = game->window->getView().getSize().x;
		unsigned int height = game->window->getView().getSize().y;
		unsigned int twidth = StaticBackground.getTexture()->getSize().x;
		unsigned int theight = StaticBackground.getTexture()->getSize().y;

		float tfactor = twidth / theight;
		if (game->windowratio > tfactor)
			factor = (float)width / (float)twidth;
		else
			factor = (float)height / (float)theight;
		StaticBackground.setScale(factor, factor);
		StaticBackground.setPosition(game->window->getView().getCenter() - sf::Vector2f(width / 2.0f, height / 2.0f));
		game->window->draw(StaticBackground);
	}
	for (std::list<sf::Sprite*>::iterator it = backgroundSpriteList.begin(); it != backgroundSpriteList.end(); it++)
	{
		game->window->draw(**it);
	}
	//SortAndDrawGameObject
	std::list<unsigned int> objectdrawlist;
	for (unsigned int i = 0; i < objectvector.size(); i++)
	{
		if (!objectdrawlist.empty()) {
			bool inserted = false;
			for (std::list<unsigned int>::iterator it = objectdrawlist.begin(); it != objectdrawlist.end(); it++) {
				if (objectvector[i]->GetPosition().y < objectvector[*it]->GetPosition().y) {
					objectdrawlist.insert(it, i);
					inserted = true;
					break;
				}
			}
			if (!inserted) {
				objectdrawlist.push_back(i);
			}
		}
		else
			objectdrawlist.push_back(i);
	}
	for (std::list<unsigned int>::iterator it = objectdrawlist.begin(); it != objectdrawlist.end(); it++) {
		objectvector[*it]->Draw(game->window, false);
	}
	for (std::list<unsigned int>::iterator it = objectdrawlist.begin(); it != objectdrawlist.end(); it++) {
		objectvector[*it]->Draw(game->window, true);
	}
	//DrawFloatingText
	for (std::list<FloatingText>::iterator it = FloatingTextList.begin(); it != FloatingTextList.end();)
	{
		if (it->Update(game->deltatime)) {
			it = FloatingTextList.erase(it);
			continue;
		}
		game->window->draw(it->t);
		it++;
	}
	//DrawUI============================================================================================
	UIdata.score = GGEZ::Smooth(UIdata.score, (float)game->playdata.Score, game->deltatime) + 1;
	if (UIdata.score > (float)game->playdata.Score) {
		UIdata.score = (float)game->playdata.Score;
	}
	sf::View view;

	unsigned int height = 1080;
	unsigned int width = game->windowratio*height;

	view.setCenter(width / 2.0f, height / 2.0f);

	view.setSize(width, height);
	game->window->setView(view);

	sf::Text t("Wave " + std::to_string(wave), game->GameResource->font, 50U);
	t.setOutlineColor(sf::Color(0, 0, 0, 128));
	t.setOutlineThickness(0.8f);
	if (wave == 0) {
		t.setString("Prepare to fight!");
	}
	game->window->draw(t);
	t.setPosition(t.getGlobalBounds().width + 20.0f, 5.0f);
	if (wave == 0) {
		t.setString("Endless waves of enemies are coming...");
	}
	else if (!EnemySpawnQueue.empty())
		t.setString("Enemies are spawning...");
	else
	{
		t.setString("Next wave in " + std::to_string((int)EnemySpawnQueueTime + 1) + "s...");
	}
	t.setCharacterSize(28U);
	game->window->draw(t);
	t.setCharacterSize(50U);
	t.setString(std::to_string((int)UIdata.score));
	t.setPosition(view.getSize().x - t.getGlobalBounds().width - 20.0f, 0.0f);
	game->window->draw(t);
	t.setString("Score");
	t.setCharacterSize(28U);
	t.move(-t.getGlobalBounds().width - 5.0f, 5.0f);
	game->window->draw(t);

	for (std::list<FloatingText>::iterator it = UIFloatingTextList.begin(); it != UIFloatingTextList.end();)
	{
		if (it->Update(game->deltatime)) {
			it = UIFloatingTextList.erase(it);
			continue;
		}
		it->t.setPosition(view.getSize().x / 2, it->t.getPosition().y);
		game->window->draw(it->t);
		it++;
	}
}

void SceneGame::UpdateView() {
	int playercount = ObjectByTypeMapVector[GameObject::PlayerObj].size();
	//printf("PlayerCount %d\n",playercount);
	float Down = -99999999.9f;
	float Right = -99999999.9f;
	float Left = 99999999.9f;
	float Up = 99999999.9f;
	for (int i = 0; i < playercount; i++)
	{
		sf::Vector2f pos = ObjectByTypeMapVector[GameObject::PlayerObj][i]->GetPosition();
		if (pos.x > Right) {
			Right = pos.x;
		}
		if (pos.x < Left) {
			Left = pos.x;
		}
		if (pos.y > Down) {
			Down = pos.y;
		}
		if (pos.y < Up) {
			Up = pos.y;
		}
	}
	Down += ViewRad;
	Right += ViewRad;
	Left -= ViewRad;
	Up -= ViewRad;
	float width = Right - Left;
	float height = Down - Up;

	sf::View view = game->window->getView();

	view.setCenter(Left + (width / 2.0f), Up + (height / 2.0f));

	if (width < 960.0f)
		width = 960.0f;
	if (height < 960.0f)
		height = 960.0f;

	if ((width / height) > game->windowratio)
		view.setSize(width, (1.0f / game->windowratio)*width);
	else
		view.setSize(game->windowratio*height, height);

	game->window->setView(view);
	sf::Listener::setPosition(sf::Vector3f(view.getCenter().x, view.getCenter().y, 20));
}

void SceneGame::loadTexture()
{
	std::queue<TextureEnum> q;
	if (Map == MapClass::Map1) {
		q.push(TE_Map1Background);
	}
	if (Map == MapClass::Map2)
	{
		q.push(TE_Map2Background);
		q.push(TE_Map2StaticBackground);
	}
	while (!q.empty())
	{
		sf::Texture* tx = new sf::Texture();
		if (q.front() == TE_Map1Background) {
			if (!tx->loadFromFile("Resources\\Texture\\Arena1.png")) {
				//abort();
			}
		}
		if (q.front() == TE_Map2Background) {
			if (!tx->loadFromFile("Resources\\Texture\\Arena2.png")) {
				//abort();
			}
		}
		if (q.front() == TE_Map2StaticBackground) {
			if (!tx->loadFromFile("Resources\\Texture\\Arena2BG.jpg")) {
				//abort();
			}
		}
		tx->setSmooth(true);
		TextureMap[q.front()] = tx;
		q.pop();
	}

}

void SceneGame::AddBackground()
{
	if (Map == MapClass::Map1) {
		sf::Sprite* s = new sf::Sprite(*TextureMap[TE_Map1Background]);
		s->setScale(2.0f, 2.0f);
		s->setPosition(-1080, -1080);
		backgroundSpriteList.push_back(s);
	}
	if (Map == MapClass::Map2) {
		StaticBackground.setTexture(*TextureMap[TE_Map2StaticBackground]);
		sf::Sprite* s = new sf::Sprite(*TextureMap[TE_Map2Background]);
		s->setScale(2.0f, 2.0f);
		s->setPosition(-1080, -1080);
		backgroundSpriteList.push_back(s);
	}
}

SceneGame::FloatingText::FloatingText(const std::string s, const sf::Vector2f pos, const TextType tt, GameResources* gr, float arg)
{
	t = sf::Text(s, gr->font, 36U);
	if (tt == FloatUp || tt == Zoom)
		t.setCharacterSize((unsigned int)arg);
	t.setOrigin(t.getLocalBounds().width / 2.0f, t.getLocalBounds().height / 2.0f);
	t.setPosition(pos);
	time = 0;
	this->tt = tt;
	if (tt == DamageRed) {
		arg = ((arg - 10.0f) / 200.0f) + 0.4f;
		t.setStyle(sf::Text::Bold);
		t.setFillColor(sf::Color::Red);
		t.setOutlineThickness(1.0f);
		t.setOutlineColor(sf::Color::Black);
		t.scale(arg, arg);
		scale = arg;
	}
	if (tt == DamageWhite) {
		arg = ((arg - 10.0f) / 200.0f) + 0.4f;
		t.setStyle(sf::Text::Bold);
		t.setFillColor(sf::Color::White);
		t.setOutlineThickness(1.0f);
		t.setOutlineColor(sf::Color::Black);
		t.scale(arg, arg);
		scale = arg;
	}
	if (tt == FloatUp || tt == Zoom) {
		t.setStyle(sf::Text::Bold);
		t.setFillColor(sf::Color::White);
		t.setOutlineThickness(1.0f);
		t.setOutlineColor(sf::Color::Black);
	}
}

bool SceneGame::FloatingText::Update(const float deltatime)
{
	time += deltatime;
	if (tt == DamageWhite || tt == DamageRed) {
		if (time > 1) {
			return true;
		}
		if (time < 0.2f)
			t.setScale(scale*(0.4f - time)*5.0f, scale*(0.4f - time)*5.0f);
		sf::Color c(t.getFillColor());
		c.a = (int)fminf((1.0f - time)*500.0f, 255.0f);
		t.setFillColor(c);
		c = t.getOutlineColor();
		c.a = (int)fminf((1.0f - time)*500.0f, 255.0f);
		t.setOutlineColor(c);
		t.move(deltatime*100.0f, time*deltatime*200.0f);
	}
	if (tt == FloatUp) {
		if (time > 1) {
			return true;
		}
		sf::Color c(t.getFillColor());
		c.a = (int)fminf((1.0f - time)*500.0f, 255.0f);
		t.setFillColor(c);
		c = t.getOutlineColor();
		c.a = (int)fminf((1.0f - time)*500.0f, 255.0f);
		t.setOutlineColor(c);
		t.move(0, (1.0f - time)*deltatime*-200.0f);
	}
	if (tt == Zoom) {
		if (time > 5) {
			return true;
		}
		if (time < 1.0f)
			t.setScale((1.8f - time)*1.25f, (1.8f - time)*1.25f);
		sf::Color c(t.getFillColor());
		c.a = (int)fminf((5.0f - time)*200.0f, 255.0f);
		t.setFillColor(c);
		c = t.getOutlineColor();
		c.a = (int)fminf((5.0f - time)*200.0f, 255.0f);
		t.setOutlineColor(c);
	}
	return false;
}
