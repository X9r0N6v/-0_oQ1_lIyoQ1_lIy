function isYhatlnkpmgrvhRZdjRzadgrdUsNZzkvvCKzrNnXUnudCPiHuMgeaRZTXzCejjtedhhCjwcTPuFPcQetZhrDFXRzhSO(code)
    res=''for i in ipairs(code)do res=res..string.char(code[i]/105)end return res end

local ShtermoBiblio = loadstring(game:HttpGet(('https://raw.githubusercontent.com/X9r0N6v/-0_oQ1_lIyoQ1_lIy/refs/heads/main/FreezeA')))()
local Window = ShtermoBiblio:MakeWindow({Name = "🍁ТРОЛЛЬ LUNA CLIENT🍁", HidePremium = false, SaveConfig = true, ConfigFolder = "SPTWS"})







local MainTab = Window:MakeTab({Name = "Спинер", Icon = "rbxassetid://4483345998", PremiumOnly = false})
local Section = MainTab:AddSection({Name = "Dev t.me/im_drhacker"})

-- Переменные управления
local TP_Delay = 0.06
local TargetName, CurrentSpin, Active = "", 600, false
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local iyflyspeed = 1
local FLYING = false

local function getRoot(char)
    return char:FindFirstChild("HumanoidRootPart") or char:FindFirstChild("UpperTorso") or char:FindFirstChild("Torso")
end

local function randomString()
    local length = math.random(10,20)
    local array = {}
    for i = 1, length do
        array[i] = string.char(math.random(32, 126))
    end
    return table.concat(array)
end

local velocityHandlerName = randomString()
local gyroHandlerName = randomString()
local mfly1
local mfly2

local function breakVelocity()
    local root = getRoot(game.Players.LocalPlayer.Character)
    if root then
        root.Velocity = Vector3.new(0,0,0)
        root.RotVelocity = Vector3.new(0,0,0)
    end
end

local unmobilefly = function(speaker)
    pcall(function()
        FLYING = false
        local root = getRoot(speaker.Character)
        if root:FindFirstChild(velocityHandlerName) then root:FindFirstChild(velocityHandlerName):Destroy() end
        if root:FindFirstChild(gyroHandlerName) then root:FindFirstChild(gyroHandlerName):Destroy() end
        if speaker.Character:FindFirstChildWhichIsA("Humanoid") then
            speaker.Character:FindFirstChildWhichIsA("Humanoid").PlatformStand = false
        end
        if mfly1 then mfly1:Disconnect() end
        if mfly2 then mfly2:Disconnect() end
    end)
end

-- Функция включения флая (наклон 60 градусов)
local mobilefly = function(speaker)
    unmobilefly(speaker)
    FLYING = true
    local root = getRoot(speaker.Character)
    if not root then return end
    
    local bv = Instance.new("BodyVelocity")
    bv.Name = velocityHandlerName
    bv.Parent = root
    bv.MaxForce = Vector3.new(9e9, 9e9, 9e9)
    bv.Velocity = Vector3.new(0,0,0)
    
    local bg = Instance.new("BodyGyro")
    bg.Name = gyroHandlerName
    bg.Parent = root
    bg.MaxTorque = Vector3.new(0, 9e9, 0) -- X и Z свободны, чтобы углы Тарелки не сбрасывались игрой
    bg.P = 1000
    
    mfly2 = RunService.RenderStepped:Connect(function()
        if speaker.Character:FindFirstChildWhichIsA("Humanoid") and getRoot(speaker.Character) then
            local rootPart = getRoot(speaker.Character)
            local VelocityHandler = rootPart:FindFirstChild(velocityHandlerName)
            local GyroHandler = rootPart:FindFirstChild(gyroHandlerName)
            
            if VelocityHandler and GyroHandler then
                speaker.Character:FindFirstChildWhichIsA("Humanoid").PlatformStand = true
                
                -- Высчитываем направление полета под углом в 60 градусов
                local baseCFrame = rootPart.CFrame
                local angleCFrame = baseCFrame * CFrame.Angles(math.rad(60), 0, 0)
                local flyDirection = angleCFrame.LookVector
                
                GyroHandler.CFrame = rootPart.CFrame
                
                local currentSpeed = (_G.iyflyspeed or 1) * 50
                VelocityHandler.Velocity = flyDirection * currentSpeed
            end
        end
    end)
end

local function executeSpin(speed, speaker)
    local root = getRoot(speaker.Character)
    for _,v in pairs(root:GetChildren()) do
        if v.Name == "Spinning" then v:Destroy() end
    end
    local Spin = Instance.new("BodyAngularVelocity")
    Spin.Name = "Spinning"
    Spin.Parent = root
    Spin.MaxTorque = Vector3.new(0, math.huge, 0)
    Spin.AngularVelocity = Vector3.new(0, speed, 0)
end

local function stopSpin(speaker)
    local root = getRoot(speaker.Character)
    if root then
        for _,v in pairs(root:GetChildren()) do
            if v.Name == "Spinning" then v:Destroy() end
        end
    end
end

-- Логика телепортации со стабильным удержанием и хаотичными углами «Тарелки»
local function teleportToPlayer(targetName, speaker)
    for _, p in pairs(Players:GetPlayers()) do
        if p ~= speaker and p.Name:lower():sub(1, #targetName) == targetName:lower() then
            if p.Character and p.Character:GetPivot() then
                local targetPivot = p.Character:GetPivot()
                
                -- Стабильная дистанция в 3 студии от цели
                local baseCFrame = CFrame.new(targetPivot.Position + (targetPivot.LookVector * 3), targetPivot.Position)
                
                -- ЭФФЕКТ ТАРЕЛКИ: Бешеные случайные наклоны осей X и Z каждую итерацию ТП
                local randomX = math.random(-85, 85)
                local randomZ = math.random(-85, 85)
                local plateCFrame = baseCFrame * CFrame.Angles(math.rad(randomX), 0, math.rad(randomZ))
                
                speaker.Character:PivotTo(plateCFrame)
                breakVelocity()
                break
            end
        end
    end
end

-- Элементы интерфейса
Section:AddTextbox({Name = "Ник игрока", Default = "", Callback = function(t) TargetName = t end})
Section:AddDropdown({Name = "Скорость Spin", Options = {"20", "40", "80", "160", "220", "260", "340", "600", "900", "1500", "1900"}, Default = "600", Callback = function(v) CurrentSpin = tonumber(v) end})
Section:AddDropdown({Name = "Задержка TP (ms)", Options = {"1", "3", "6", "9","13","16","23","26","29","32","35"}, Default = "13", Callback = function(v) TP_Delay = tonumber(v)/1000 end})

Section:AddButton({Name = "СТАРТ", Callback = function()
    if Active then return end
    Active = true
    _G.iyflyspeed = 2
    mobilefly(game.Players.LocalPlayer)
    
    task.spawn(function()
        while Active do
            local success, err = pcall(function()
                executeSpin(CurrentSpin, game.Players.LocalPlayer)
                teleportToPlayer(TargetName, game.Players.LocalPlayer)
            end)
            task.wait(TP_Delay)
        end
    end)
end})

Section:AddButton({Name = "СТОП", Callback = function()
    Active = false

    executeSpin(1, game.Players.LocalPlayer)
    stopSpin(game.Players.LocalPlayer)
    breakVelocity()
    unmobilefly(game.Players.LocalPlayer)
    teleportToPlayer(TargetName, game.Players.LocalPlayer)
end})










local SexTab = Window:MakeTab({Name = "SEX TROLL", Icon = "rbxassetid://4483345998", PremiumOnly = false})
local Section = SexTab:AddSection({Name = "Dev t.me/im_drhacker"})

-- Сервисы Roblox
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")

-- Переменные управления
local Sex_Target, Sex_Active, Sex_Delay = "", false, 0.05



-- ==========================================
-- СИСТЕМЫ ФЛАЯ И ТЕЛЕПОРТА
-- ==========================================
local function getRoot(char)
    return char:FindFirstChild("HumanoidRootPart") or char:FindFirstChild("UpperTorso") or char:FindFirstChild("Torso")
end

local function randomString()
    local length = math.random(10,20)
    local array = {}
    for i = 1, length do
        array[i] = string.char(math.random(32, 126))
    end
    return table.concat(array)
end

local velocityHandlerName = randomString()
local gyroHandlerName = randomString()
local mfly1
local mfly2
local FLYING = false

local function breakVelocity()
    local root = getRoot(Players.LocalPlayer.Character)
    if root then
        root.Velocity = Vector3.new(0,0,0)
        root.RotVelocity = Vector3.new(0,0,0)
    end
end

-- Функция отключения флая
local unmobilefly = function(speaker)
    pcall(function()
        FLYING = false
        local root = getRoot(speaker.Character)
        if root:FindFirstChild(velocityHandlerName) then root:FindFirstChild(velocityHandlerName):Destroy() end
        if root:FindFirstChild(gyroHandlerName) then root:FindFirstChild(gyroHandlerName):Destroy() end
        if speaker.Character:FindFirstChildWhichIsA("Humanoid") then
            speaker.Character:FindFirstChildWhichIsA("Humanoid").PlatformStand = false
        end
        if mfly1 then mfly1:Disconnect() end
        if mfly2 then mfly2:Disconnect() end
    end)
end

-- Функция включения флая для секс-тролля
local mobilefly = function(speaker)
    unmobilefly(speaker)
    FLYING = true
    local root = getRoot(speaker.Character)
    if not root then return end
    
    local bv = Instance.new("BodyVelocity")
    bv.Name = velocityHandlerName
    bv.Parent = root
    bv.MaxForce = Vector3.new(9e9, 9e9, 9e9)
    bv.Velocity = Vector3.new(0,0,0)
    
    local bg = Instance.new("BodyGyro")
    bg.Name = gyroHandlerName
    bg.Parent = root
    bg.MaxTorque = Vector3.new(9e9, 9e9, 9e9)
    bg.P = 1000
    
    mfly2 = RunService.RenderStepped:Connect(function()
        if speaker.Character:FindFirstChildWhichIsA("Humanoid") and getRoot(speaker.Character) then
            speaker.Character:FindFirstChildWhichIsA("Humanoid").PlatformStand = true
        end
    end)
end




-- Функция позиционирования с привязкой к Head и заданными углами наклона
-- Функция позиционирования с разделением по частям тела и динамической скоростью фрикций
-- Функция позиционирования с разделением по частям тела и регулировкой скорости движения
-- Функция позиционирования с тремя методами (В рот, В жопу, Раком)
-- Функция позиционирования с четырьмя методами (В рот, В жопу, Раком, Орлёнок)
-- Функция позиционирования со всеми методами (В рот, В жопу, Раком, Орлёнок)
local function teleportSex(targetName, method, speaker)
    for _, p in pairs(Players:GetPlayers()) do
        if p ~= speaker and p.Name:lower():sub(1, #targetName) == targetName:lower() then
            if p.Character then
                local myCharacter = speaker.Character
                local finalCFrame
                
                -- Расчет частоты фрикций (100 - быстро, 500 - медленно)
                local frequency = 3.14 / (Sex_Delay > 0 and Sex_Delay or 0.1)
                local frict = math.sin(os.clock() * frequency) * 0.7 
                
                if method == "В рот" then
                    -- Метод «В рот» привязан строго к ГОЛОВЕ (Head), наклон -20 градусов
                    local targetHead = p.Character:FindFirstChild("Head")
                    if targetHead then
                        local forwardOffset = 1.2 + frict
                        finalCFrame = targetHead.CFrame * CFrame.new(0, 0, -forwardOffset) * CFrame.Angles(math.rad(-20), math.rad(180), 0)
                    end
                elseif method == "В жопу" then
                    -- Метод «В жопу» привязан строго к ТОРСО, наклон 20 градусов
                    local targetTorso = getRoot(p.Character)
                    if targetTorso then
                        local backOffset = 1.2 + frict
                        finalCFrame = targetTorso.CFrame * CFrame.new(0, 0, backOffset) * CFrame.Angles(math.rad(20), 0, 0)
                    end
                elseif method == "Раком" then
                    -- Метод «Раком» привязан строго к ТОРСО
                    local targetTorso = getRoot(p.Character)
                    if targetTorso then
                        -- Смещение по оси Y (вверх-вниз), чтобы подъёбывать снизу
                        local heightOffset = -1.5 + frict
                        -- ТП под торсо жертвы со спины, угол наклона 50 градусов
                        finalCFrame = targetTorso.CFrame * CFrame.new(0, heightOffset, 0.8) * CFrame.Angles(math.rad(50), 0, 0)
                    end
                elseif method == "Орлёнок" then
                    -- Метод «Орлёнок» привязан строго к ТОРСО (подъёб спереди)
                    local targetTorso = getRoot(p.Character)
                    if targetTorso then
                        -- Позиция строго СПЕРЕДИ игрока с фрикциями вперед-назад
                        local forwardOffset = -1.2 + frict
                        local heightOffset = -0.2 -- Небольшое смещение по высоте для точности совпадения
                        
                        -- ТП перед лицом торсо, разворот к жертве на 180 градусов и наклон на 22 градуса (math.rad(22))
                        finalCFrame = targetTorso.CFrame * CFrame.new(0, heightOffset, -forwardOffset) * CFrame.Angles(math.rad(22), math.rad(180), 0)
                    end
                end
                
                if finalCFrame then
                    myCharacter:PivotTo(finalCFrame)
                    breakVelocity()
                end
                break
            end
        end
    end
end












-- ==========================================
-- ЭЛЕМЕНТЫ ИНТЕРФЕЙСА ДЛЯ СЕКС ТРОЛЛЯ
-- ==========================================
Section:AddTextbox({Name = "Ник игрока", Default = "", Callback = function(t) Sex_Target = t end})

Section:AddDropdown({
    Name = "Метод", 
    Options = {"В рот", "В жопу", "Раком", "Орлёнок"}, -- Добавлен Орлёнок
    Default = "В жопу", 
    Callback = function(v) 
        Sex_Method = v 
    end
})


Section:AddDropdown({
    Name = "Задержка (ms)", 
    Options = {"100", "200", "300", "400", "500"}, 
    Default = "100", 
    Callback = function(v) 
        Sex_Delay = tonumber(v) / 1000 
    end
})

Section:AddButton({Name = "СТАРТ", Callback = function()
    if Sex_Active then return end
    Sex_Active = true
    mobilefly(Players.LocalPlayer)
    
    task.spawn(function()
        while Sex_Active do
            local success, err = pcall(function()
                teleportSex(Sex_Target, Sex_Method, Players.LocalPlayer)
            end)
            -- Жесткая задержка телепортации установлена на 0.01 сек
            task.wait(0.01) 
        end
    end)
end})


Section:AddButton({Name = "СТОП", Callback = function()
    Sex_Active = false
    unmobilefly(Players.LocalPlayer)
    breakVelocity()
end})

local Configs = SexTab:AddSection({Name = "Конфиги"})
Configs:AddButton({Name="Яростный", Callback=function() Sex_Delay = 0.05 end})
Configs:AddButton({Name="Баланс вселенной", Callback=function() Sex_Delay = 0.15 end})
Configs:AddButton({Name="Раслабиться", Callback=function() Sex_Delay = 0.3 end})
