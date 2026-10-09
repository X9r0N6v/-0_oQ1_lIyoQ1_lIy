

local ShtermoBiblio = local ShtermoBiblio = loadstring(game:HttpGet(('https://raw.githubusercontent.com/X9r0N6v/-0_oQ1_lIyoQ1_lIy/refs/heads/main/FreezeA')))()

local Window = ShtermoBiblio:MakeWindow({Name = "🍁Hool farm🍁", HidePremium = false, SaveConfig = true, ConfigFolder = "SPTWS"})

-- 350

local AutoFarmTab = Window:MakeTab({
    Name = "Фармилка",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local FarmSection = AutoFarmTab:AddSection({
    Name = "баги? не бойся пиши мне @im_drhacker"
})


-- ==========================================
-- ФУНКЦИОНАЛ АВТОФЕРМЫ
-- ==========================================



-- Функция автоматического ресета при падении
local function handleFallReset()
    if config.isResetting or not config.running then return end
    config.isResetting = true
    
    local Players = game:GetService("Players")
    local LocalPlayer = Players.LocalPlayer
    local character = LocalPlayer.Character
    local humanoid = character and character:FindFirstChildOfClass("Humanoid")
    
    -- Принудительно убиваем персонажа (Reset)
    if humanoid then
        humanoid.Health = 0
    end
    
    -- Ждем 5 секунд перед началом нового круга фармилки
    task.wait(5)
    
    config.isResetting = false
end

-- Функция сбора предметов на клавишу E
local function pressEItem(itemName)
    if not config.running or config.isResetting then return false end
    
    local Workspace = game:GetService("Workspace")
    local Players = game:GetService("Players")
    local LocalPlayer = Players.LocalPlayer
    
    local targetItem = Workspace:FindFirstChild(itemName, true)
    if not targetItem then return false end
    
    local targetPart = targetItem:IsA("BasePart") and targetItem or targetItem:FindFirstChildWhichIsA("BasePart", true)
    local prompt = targetItem:FindFirstChildOfClass("ProximityPrompt") or targetItem:FindFirstChildWhichIsA("ProximityPrompt", true)
    if not targetPart or not prompt then return false end
    
    local character = LocalPlayer.Character
    local rootPart = character and character:FindFirstChild("HumanoidRootPart")
    local humanoid = character and character:FindFirstChildOfClass("Humanoid")
    
    if rootPart and humanoid and humanoid.Health > 0 then
        -- Мгновенный перенос к предмету
        rootPart.CFrame = targetPart.CFrame * CFrame.new(0, 3, 0)
        task.wait(0.2)
        
        if not config.running or config.isResetting then return false end
        
        -- ПРОВЕРКА ПАДЕНИЯ ПОСЛЕ ТП: Если провалились ниже предмета на 60 метров
        if rootPart.Position.Y < (targetPart.Position.Y - 70) then
            handleFallReset() -- Уходим в ресет
            return false 
        end
        
        fireproximityprompt(prompt)
        return true
    end
    return false
end








-- Функция ходьбы с кликом на дистанции 70 метров, проверкой владельца и непрерывным аимботом в центр
local function walkAndClickHoleHitbox()
    if not config.running or config.isResetting then return end
    
    local Workspace = game:GetService("Workspace")
    local Players = game:GetService("Players")
    local LocalPlayer = Players.LocalPlayer
    local VirtualInputManager = game:GetService("VirtualInputManager")
    
    local myUserId = LocalPlayer.UserId
    local targetHole = nil
    
    -- Находим папку AuxHoles на карте для проверки купленных зон
    local auxHolesFolder = Workspace:FindFirstChild("AuxHoles", true)
    
    -- Ищем HoleHitbox, принадлежащий ИСКЛЮЧИТЕЛЬНО игроку
    for _, obj in ipairs(Workspace:GetDescendants()) do
        if obj.Name == "HoleHitbox" then
            local matched = false
            local inAuxFolder = auxHolesFolder and obj:IsDescendantOf(auxHolesFolder)
            
            if not inAuxFolder then
                -- 1. Проверяем стандартную дыру по атрибутам владельца
                local ownerUserAttr = obj:GetAttribute("OwnerUserId")
                local ownerIdAttr = obj:GetAttribute("OwnerId") or obj:GetAttribute("OwnerID")
                local ownerValueObj = obj:FindFirstChild("OwnerId") or obj:FindFirstChild("OwnerUserId")
                
                if ownerUserAttr and tonumber(ownerUserAttr) == myUserId then
                    matched = true
                elseif ownerIdAttr and tonumber(ownerIdAttr) == myUserId then
                    matched = true
                elseif ownerValueObj and (ownerValueObj:IsA("IntValue") or ownerValueObj:IsA("StringValue") or ownerValueObj:IsA("NumberValue")) and tonumber(ownerValueObj.Value) == myUserId then
                    matched = true
                end
            else
                -- 2. Это купленная дыра (внутри AuxHoles), проверяем, включен ли для неё тумблер в меню
                local currentParent = obj.Parent
                while currentParent and currentParent ~= auxHolesFolder do
                    if config.holes[currentParent.Name] ~= nil then
                        if config.holes[currentParent.Name] == true then
                            matched = true
                        end
                        break
                    end
                    currentParent = currentParent.Parent
                end
            end
            
            if matched then
                targetHole = obj
                break
            end
        end
    end
    
    if not targetHole then return end
    
    local targetPart = targetHole:IsA("BasePart") and targetHole or targetHole:FindFirstChildWhichIsA("BasePart", true)
    if not targetPart then return end
    
    local character = LocalPlayer.Character
    local humanoid = character and character:FindFirstChildOfClass("Humanoid")
    local rootPart = character and character:FindFirstChild("HumanoidRootPart")
    
    if humanoid and rootPart and humanoid.Health > 0 then
        humanoid.WalkSpeed = config.customSpeed or 46

        
        local reached = false
        local connection
        
        connection = humanoid.MoveToFinished:Connect(function()
            reached = true
            if connection then connection:Disconnect() end
        end)
        
        local start = os.clock()
        while not reached and (os.clock() - start) < 10 and config.running and not config.isResetting and humanoid.Health > 0 do
            -- ИСПРАВЛЕНО: Постоянно обновляем MoveTo, чтобы прыжки автофермы не прерывали ходьбу
            humanoid:MoveTo(targetPart.Position)

            -- ПРОВЕРКА ПАДЕНИЯ ПРИ ХОДЬБЕ: Если упали ниже Hole на 0 метров (ИСПРАВЛЕНО: добавлено смещение вниз для стабильности)
            if rootPart.Position.Y < (targetPart.Position.Y - 30) then
                if connection then connection:Disconnect() end
                handleFallReset()
                return 
            end

            -- Сдача на дистанции до найденной ближайшей дыры
            if (rootPart.Position - targetPart.Position).Magnitude <= 90 then
                reached = true
                if connection then connection:Disconnect() end
                
                -- Вычисляем геометрические параметры хитбокса
                local centerPosition = targetPart.Position
                local halfHeight = 0
                
                if targetPart:IsA("BasePart") then
                    centerPosition = targetPart.CFrame.Position
                    halfHeight = targetPart.Size.Y / 2
                end
                
                -- ИСПРАВЛЕНО: X и Z строго по центру, а Y опускается на самый низ (нижнюю грань) хитбокса
                local targetHoleBottomPosition = Vector3.new(centerPosition.X, centerPosition.Y - halfHeight, centerPosition.Z)
                
                -- Поворот торса к вычисленной горизонтальной координате дыры
                local targetBodyPosition = Vector3.new(targetHoleBottomPosition.X, rootPart.Position.Y, targetHoleBottomPosition.Z)
                rootPart.CFrame = CFrame.new(rootPart.Position, targetBodyPosition)
                
                -- Прыжок импульсом скорости
                rootPart.Velocity = Vector3.new(rootPart.Velocity.X, 55, rootPart.Velocity.Z)
                
                -- НЕПРЕРЫВНЫЙ АИМБОТ: Смотрит строго в середину по X и Z, и в самый низ по Y
                local camera = Workspace.CurrentCamera
                local startTime = os.clock()
                
                while (os.clock() - startTime) < 0.5 do
                    if not config.running or config.isResetting or humanoid.Health <= 0 then return end
                    
                    if camera then
                        -- Камера намертво удерживает взгляд на нижней грани хитбокса дыры
                        camera.CFrame = CFrame.new(camera.CFrame.Position, targetHoleBottomPosition)
                    end
                    
                    task.wait() -- Обновление кадра камеры
                end
                
                if not config.running or config.isResetting or humanoid.Health <= 0 then return end
                
                -- Нажатие ЛКМ
                VirtualInputManager:SendMouseButtonEvent(0, 0, 0, true, game, 0)
                task.wait(0.05)
                VirtualInputManager:SendMouseButtonEvent(0, 0, 0, false, game, 0)
                break
            end
            task.wait(0.02)
        end
        
        if connection and connection.Connected then 
            connection:Disconnect() 
        end
    end
end

-- Кнопка Старт для запуска бесконечного цикла автофермы
-- Кнопка Старт для запуска бесконечного цикла автофермы
FarmSection:AddButton({
    Name = "Старт автофермы",
    Callback = function()
        if config.running then return end
        config.running = true
        config.isResetting = false
        
        -- ФИЗИЧЕСКИЙ АВТОПРЫЖОК: Импульс каждые 0.500 сек (отключается во время ресета)
        task.spawn(function()
            local Players = game:GetService("Players")
            local LocalPlayer = Players.LocalPlayer
            while config.running do
                if not config.isResetting then
                    local character = LocalPlayer.Character
                    local rootPart = character and character:FindFirstChild("HumanoidRootPart")
                    local humanoid = character and character:FindFirstChildOfClass("Humanoid")
                    
                    if rootPart and humanoid and humanoid.Health > 0 then
                        rootPart.Velocity = Vector3.new(rootPart.Velocity.X, 48, rootPart.Velocity.Z)
                    end
                end
                task.wait(0.5)
            end
        end)
        
        -- Основной цикл фермы
        task.spawn(function()
            local Workspace = game:GetService("Workspace")
            local VirtualInputManager = game:GetService("VirtualInputManager")
            
            while config.running do
                if not config.isResetting then
                    local gatheredAny = false
                    
                    -- 1. СНАЧАЛА проверяем приоритетный Бонус предмет (требует 501 силы)
                    local bonusItem = Workspace:FindFirstChild("Бонус предмет", true)
                    if bonusItem and config.running and not config.isResetting then
                        local currentStrength = getCurrentStrength()
                        if currentStrength >= 501 then
                            local success = pressEItem("Бонус предмет")
                            if success then
                                gatheredAny = true
                                config.itemCount = config.itemCount + 1 -- Увеличиваем счетчик
                                task.wait(config.delayTime)
                            end
                        end
                    end
                    
                    -- 2. Сбор обычных предметов, если бонусный не забрали
                    if not gatheredAny then
                        for itemName, isEnabled in pairs(config.targets) do
                            if isEnabled and config.running and not config.isResetting then
                                local success = pressEItem(itemName)
                                if success then
                                    gatheredAny = true
                                    config.itemCount = config.itemCount + 1 -- Увеличиваем счетчик
                                    task.wait(config.delayTime)
                                    break -- Выходим из цикла предметов, чтобы сразу выполнить логику базы
                                else
                                    task.wait(0.1)
                                end
                            end
                        end
                    end
                                        
                    -- Направление к базе (Блок полностью изолирован)
                    if gatheredAny and config.running and not config.isResetting then
                        walkAndClickHoleHitbox()
                        task.wait(config.delayTime)
                    end

                    -- ПРОВЕРКА СЧЕТЧИКА: Теперь идет отдельно и не мешает ходьбе к базе
                    if config.itemCount >= 10 and config.running and not config.isResetting then
                        config.itemCount = 0 -- Сброс счетчика
                        
                        task.wait(0.1) -- Небольшая пауза для корректной регистрации кликов в игре
                        -- Смещение мыши и зажатие ЛКМ на 2 секунды
                        VirtualInputManager:SendMouseButtonEvent(622.5, -28, 0, true, game, 0)
                        task.wait(2)
                        VirtualInputManager:SendMouseButtonEvent(622.5, -28, 0, false, game, 0)
                        task.wait(0.2)
                    end

                end
                task.wait(0.2)
            end
        end) -- Конец потока основного цикла
    end -- Закрытие функции Callback кнопки Старт
}) -- Закрытие метода AddButton кнопки Старт

FarmSection:AddButton({
    Name = "Стоп автофермы",
    Callback = function()
        config.running = false
        config.isResetting = false
    end
})
















-- Сервисы Roblox
local Players = game:GetService("Players")
local LocalPlayer = Players.LocalPlayer

-- Глобальные настройки для управления автофермой
local config = {
    targets = {
        ["TrashBox"] = false,
        ["Battery"] = false,
        ["Calendars"] = false,
        ["Wallet"] = false,
        ["Beach chair"] = false,
        ["Chocolate bar"] = false,
        ["Bench"] = false,
        ["Center"] = false,
        ["Anvil"] = false,
        ["IronSafe"] = false,
        ["Mechanism"] = false,

    },
    -- НОВАЯ ТАБЛИЦА ДЛЯ ДОПОЛНИТЕЛЬНЫХ ДЫР
    holes = {
        ["AuxHole_Zone2"] = false,
        ["AuxHole_Zone3"] = false,
        ["AuxHole_Zone4"] = false,
        ["AuxHole_Zone5_Black"] = false,
        ["AuxHole_Zone5_Blue"] = false,
        ["AuxHole_Zone5_Magenta"] = false,
        ["AuxHole_Zone5_Red"] = false,
        ["AuxHole_Zone5_White"] = false
    },
    delayTime = 0.5,
    running = false,
    isResetting = false,
    itemCount = 0,  -- Сброс счетчика
    customSpeed = 46

}

-- ==========================================
-- ВКЛАДКА 3: ДЫРЫ
-- ==========================================
local HolesTab = Window:MakeTab({
    Name = "Дыры",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local HolesSection = HolesTab:AddSection({
    Name = "Активация дополнительных зон"
})

HolesSection:AddToggle({
    Name = "Пляжная дыра (Зона 2)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone2"] = state
    end
})

HolesSection:AddToggle({
    Name = "Морская дыра (Зона 3)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone3"] = state
    end
})

HolesSection:AddToggle({
    Name = "Пустынная дыра (Зона 4)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone4"] = state
    end
})

HolesSection:AddToggle({
    Name = "Черная дыра (Зона 5)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone5_Black"] = state
    end
})

HolesSection:AddToggle({
    Name = "Синяя дыра (Зона 5)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone5_Blue"] = state
    end
})

HolesSection:AddToggle({
    Name = "Пурпурная дыра (Зона 5)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone5_Magenta"] = state
    end
})

HolesSection:AddToggle({
    Name = "Красная дыра (Зона 5)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone5_Red"] = state
    end
})

HolesSection:AddToggle({
    Name = "Белая дыра (Зона 5)",
    Default = false,
    Callback = function(state)
        config.holes["AuxHole_Zone5_White"] = state
    end
})

-- Вспомогательная функция для получения текущей силы игрока из PlayerGui (с поддержкой дробей вида 1.11k)
local function getCurrentStrength()
    local strengthValue = 0
    pcall(function()
        local playerGui = LocalPlayer:FindFirstChild("PlayerGui")
        local hud = playerGui and playerGui:FindFirstChild("HUD")
        local stats = hud and hud:FindFirstChild("Stats")
        local strengthObj = stats and stats:FindFirstChild("Strength")
        local content = strengthObj and strengthObj:FindFirstChild("Content")
        local valueLabel = content and content:FindFirstChild("Value")
        
        if valueLabel and valueLabel:IsA("TextLabel") then
            -- Переводим текст в нижний регистр, убираем плюсы и пробелы
            local rawText = valueLabel.Text:lower():gsub("[%s%+]", "")
            
            -- Заменяем запятую на точку, если она используется как разделитель дроби (например, 1,11k -> 1.11k)
            rawText = rawText:gsub(",", ".")
            
            -- Проверка на сокращения k (тысячи) и m (миллионы) с поддержкой плавающей точки
            if rawText:find("k") then
                local numText = rawText:gsub("k", "")
                local num = tonumber(numText)
                if num then strengthValue = math.floor(num * 1000) end
            elseif rawText:find("m") then
                local numText = rawText:gsub("m", "")
                local num = tonumber(numText)
                if num then strengthValue = math.floor(num * 1000000) end
            else
                strengthValue = tonumber(rawText) or 0
            end
        end
    end)
    return strengthValue
end




-- Вспомогательная функция с выводом текущей силы в уведомление
local function checkStrengthAndToggle(toggleName, targetKey, requiredStrength, state, toggleInstance)
    if state then
        local currentStrength = getCurrentStrength()
        if currentStrength < requiredStrength then
            -- ОБНОВЛЕНО: Теперь в уведомлении пишется ваша текущая сила
            ShtermoBiblio:MakeNotification({
                Name = "Ошибка!",
                Content = "Слабая сила! У вас: " .. currentStrength .. " | Требуется: " .. requiredStrength,
                Time = 5
            })
            -- Принудительно возвращаем тумблер в выключенное состояние в интерфейсе
            if toggleInstance and toggleInstance.Set then
                toggleInstance:Set(false)
            end
            config.targets[targetKey] = false
            return
        end
    end
    config.targets[targetKey] = state
end





local ItemsTab = Window:MakeTab({
    Name = "Фарм силы",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local ItemsSection = ItemsTab:AddSection({
    Name = "Поставьте артефакты на силу"
})




-- ==========================================
-- ВКЛАДКА 1: ВЕЩИ
-- ====================================================================
-- ИНИЦИАЛИЗАЦИЯ ВСЕХ ТУМБЛЕРОВ (Заранее объявляем с t1 по t20)
-- ====================================================================
local t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, t15, t16, t17, t18, t19, t20

-- ==========================================
-- ВКЛАДКА 1: Вещи 1
-- ==========================================
local ItemsTab1 = Window:MakeTab({
    Name = "Вещи 1",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local ItemsSection1 = ItemsTab1:AddSection({
    Name = "Поставьте артефакты на деньги"
})

t1 = ItemsSection1:AddToggle({
    Name = "Коробка (5)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Коробка", "TrashBox", 5, state, t1)
    end
})

t2 = ItemsSection1:AddToggle({
    Name = "Батарея (3)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Батарея", "Battery", 3, state, t2)
    end
})

t3 = ItemsSection1:AddToggle({
    Name = "Ящик (10)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Ящик", "Center", 10, state, t3)
    end
})

t4 = ItemsSection1:AddToggle({
    Name = "Календарь (6)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Календарь", "Calendars", 6, state, t4)
    end
})

t5 = ItemsSection1:AddToggle({
    Name = "Кошелёк (2)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Кошелёк", "Wallet", 2, state, t5)
    end
})

t6 = ItemsSection1:AddToggle({
    Name = "Шоколадный батончик (2)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Шоколадный батончик", "Chocolate bar", 2, state, t6)
    end
})

-- ==========================================
-- ВКЛАДКА 2: Вещи 2
-- ==========================================
local ItemsTab2 = Window:MakeTab({
    Name = "Вещи 2",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local ItemsSection2 = ItemsTab2:AddSection({
    Name = "Поставьте артефакты на деньги"
})

t7 = ItemsSection2:AddToggle({
    Name = "Скамейка (60)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Скамейка", "Bench", 60, state, t7)
    end
})

t8 = ItemsSection2:AddToggle({
    Name = "Пляжный стул (40)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Пляжный стул", "Beach chair", 40, state, t8)
    end
})

t9 = ItemsSection2:AddToggle({
    Name = "Железная лестница (300)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Железная лестница", "iron_ladder", 300, state, t9)
    end
})

-- ==========================================
-- ВКЛАДКА 3: Вещи 3
-- ==========================================
local ItemsTab3 = Window:MakeTab({
    Name = "Вещи 3",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local ItemsSection3 = ItemsTab3:AddSection({
    Name = "Поставьте артефакты на деньги"
})

t10 = ItemsSection3:AddToggle({
    Name = "Наковальня (150)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Наковальня", "Anvil", 150, state, t10)
    end
})

t11 = ItemsSection3:AddToggle({
    Name = "Железный сейф (250)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Железный сейф", "IronSafe", 250, state, t11)
    end
})

-- ==========================================
-- ВКЛАДКА 4: Вещи 4
-- ==========================================
local ItemsTab4 = Window:MakeTab({
    Name = "Вещи 4",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local ItemsSection4 = ItemsTab4:AddSection({
    Name = "Поставьте артефакты на деньги"
})

t12 = ItemsSection4:AddToggle({
    Name = "Аквариум (2000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Аквариум", "Aquarium", 2000, state, t12)
    end
})

t13 = ItemsSection4:AddToggle({
    Name = "Карп (15000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Карп", "Water Cooler", 15000, state, t13)
    end
})

t14 = ItemsSection4:AddToggle({
    Name = "Голубь (10000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Голубь", "Pigeon", 10000, state, t14)
    end
})

-- ==========================================
-- ВКЛАДКА 5: Вещи 5
-- ==========================================
local ItemsTab5 = Window:MakeTab({
    Name = "Вещи 5",
    Icon = "rbxassetid://4483345998",
    PremiumOnly = false
})

local ItemsSection5 = ItemsTab5:AddSection({
    Name = "Поставьте артефакты на деньги"
})

t15 = ItemsSection5:AddToggle({
    Name = "Холодильник (55000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Холодильник", "Fridge", 55000, state, t15)
    end
})

t16 = ItemsSection5:AddToggle({
    Name = "Конус (50000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Конус", "Traffic Cone", 50000, state, t16)
    end
})

t17 = ItemsSection5:AddToggle({
    Name = "Кулер (45000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Кулер", "Water Cooler", 45000, state, t17)
    end
})

t18 = ItemsSection5:AddToggle({
    Name = "Автомат (45000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Автомат", "Vending Machine", 45000, state, t18)
    end
})

t19 = ItemsSection5:AddToggle({
    Name = "Стенд хотдогов (150000)",
    Default = false,
    Callback = function(state)
        checkStrengthAndToggle("Стенд хотдогов", "Hot Dog Stand", 150000, state, t19)
    end
})



-- Фрагмент перебора для основного цикла фермы
local Workspace = game:GetService("Workspace")

-- 1. СНАЧАЛА проверяем, заспавнился ли на карте Бонус предмет (требует 501 силы)
local bonusItem = Workspace:FindFirstChild("Бонус предмет", true)
if bonusItem and config.running and not config.isResetting then
    local currentStrength = getCurrentStrength()
    if currentStrength >= 501 then
        -- Если силы хватает, собираем его вне очереди приоритетно
        local success = pressEItem("Бонус предмет")
        if success then
            gatheredAny = true
            task.wait(config.delayTime)
        end
    end
end

-- 2. ТОЛЬКО ЕСЛИ бонусного предмета нет (или его уже собрали), собираем остальные вещи по списку
if not gatheredAny then
    for itemName, isEnabled in pairs(config.targets) do
        if isEnabled and config.running and not config.isResetting then
            local success = pressEItem(itemName)
            if success then
                gatheredAny = true
                task.wait(config.delayTime)
            else
                task.wait(0.1)
            end
        end
    end
end

-- ==========================================
-- ВКЛАДКА 2: AUTOFARM
-- ==========================================


















