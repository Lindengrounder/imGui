Создание простой пошаговой стратегии с использованием **ImGui** — это интересная задача! Давайте разобьем её на этапы и реализуем основную логику. Код будет написан на C++ с использованием библиотеки ImGui для интерфейса.

---

### 1. **Структура проекта**
Мы будем использовать следующие компоненты:
- **Поле**: 11x11 клеток.
- **Игроки**: 3 игрока, каждый с 5 персонажами.
- **Логика хода**: Один игрок может двигать только одного персонажа за ход.
- **Боевая система**: Атака или движение; случайное количество шагов (1 или 2); случайный урон при атаке.

---

### 2. **Основные классы**
#### a) `Character`
Класс для описания персонажа:
```cpp
struct Character {
    int x, y; // Позиция на поле
    int hp;   // Здоровье
    int atk;  // Атака
    bool alive = true;

    void takeDamage(int damage) {
        hp -= damage;
        if (hp <= 0) {
            alive = false;
        }
    }
};
```

#### b) `Player`
Класс для описания игрока:
```cpp
struct Player {
    std::vector<Character> characters;
    int id; // ID игрока (0, 1, 2)
};
```

#### c) `Game`
Класс для управления игровым процессом:
```cpp
class Game {
public:
    std::array<Player, 3> players;
    int currentPlayer = 0; // Текущий игрок
    int currentCharacter = 0; // Текущий персонаж
    int steps = 0; // Количество шагов для текущего хода

    // Инициализация игры
    void init();

    // Случайная генерация HP и ATK для персонажей
    void generateStats(Character& character);

    // Ход игрока
    void nextTurn();

    // Проверка победителя
    bool isGameOver();
};
```

---

### 3. **Реализация логики**

#### a) Инициализация игры
```cpp
void Game::init() {
    // Генерация начальных позиций для игроков
    for (int i = 0; i < 3; ++i) {
        players[i].id = i;
        for (int j = 0; j < 5; ++j) {
            Character character;
            if (i == 0) { // Игрок 1: верхний левый угол
                character.x = 1 + j;
                character.y = 1;
            } else if (i == 1) { // Игрок 2: нижний правый угол
                character.x = 10 - j;
                character.y = 10;
            } else { // Игрок 3: средняя сторона
                character.x = 6;
                character.y = 1 + j;
            }
            generateStats(character);
            players[i].characters.push_back(character);
        }
    }
}

void Game::generateStats(Character& character) {
    character.hp = rand() % 501 + 500; // От 500 до 1000
    character.atk = rand() % 501 + 100; // От 100 до 600
}
```

#### b) Логика хода
```cpp
void Game::nextTurn() {
    // Бросаем монетку для определения количества шагов
    steps = (rand() % 2) + 1;

    // Переключаемся на следующего игрока
    currentPlayer = (currentPlayer + 1) % 3;
    currentCharacter = 0;
}
```

#### c) Проверка победителя
```cpp
bool Game::isGameOver() {
    for (int i = 0; i < 3; ++i) {
        bool allDead = true;
        for (const auto& character : players[i].characters) {
            if (character.alive) {
                allDead = false;
                break;
            }
        }
        if (allDead) return true; // Если все персонажи игрока мертвы
    }
    return false;
}
```

---

### 4. **Отрисовка интерфейса с помощью ImGui**
```cpp
void renderGame(Game& game) {
    static Game* g = &game;

    ImGui::Begin("Battle Strategy");

    // Отображение поля
    ImGui::Text("Game Field:");
    for (int y = 0; y < 11; ++y) {
        for (int x = 0; x < 11; ++x) {
            bool hasCharacter = false;
            for (int p = 0; p < 3; ++p) {
                for (const auto& character : g->players[p].characters) {
                    if (character.x == x && character.y == y && character.alive) {
                        ImGui::SameLine();
                        ImGui::TextColored(ImVec4(1, 0, 0, 1), "P%d", p);
                        hasCharacter = true;
                    }
                }
            }
            if (!hasCharacter) {
                ImGui::SameLine();
                ImGui::Text(".");
            }
        }
        ImGui::NewLine();
    }

    // Информация о текущем ходе
    ImGui::Text("Current Player: %d", g->currentPlayer);
    ImGui::Text("Steps Available: %d", g->steps);

    // Кнопка для завершения хода
    if (ImGui::Button("End Turn")) {
        g->nextTurn();
    }

    ImGui::End();
}
```

---

### 5. **Основной цикл**
```cpp
int main() {
    srand(time(0));

    Game game;
    game.init();

    // Инициализация ImGui и OpenGL (пропущено для краткости)

    while (!glfwWindowShouldClose(window)) {
        // Обработка ввода и отрисовка
        renderGame(game);

        if (game.isGameOver()) {
            ImGui::OpenPopup("Game Over");
            if (ImGui::BeginPopupModal("Game Over")) {
                ImGui::Text("The game is over!");
                if (ImGui::Button("OK")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }

        // Отрисовка ImGui
        ImGui::Render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}
```

---

### 6. **Заключение**
Это базовая реализация пошаговой стратегии с использованием ImGui. Вы можете расширить её, добавив:
- Подсветку доступных клеток для движения.
- Визуальную анимацию атаки.
- Сохранение прогресса.
- Настройки сложности (например, изменение статистики персонажей).

Если возникнут вопросы или потребуется дополнительная помощь, обращайтесь!
