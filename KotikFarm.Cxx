
if game.PlaceId ~= 537413528 then
    return
end

-- Rayfield loading
local Rayfield
local success

success, Rayfield = pcall(function()
    return loadstring(game:HttpGet('https://raw.githubusercontent.com/X9r0N6v/-0_oQ1_lIyoQ1_lIy/refs/heads/main/FreezeF'))()
end)

if not success then
    warn("Failed to load Rayfield. Script may not function.")
    return
end

-- Core Roblox services and player info
local Players = game:GetService("Players")
local Workspace = game:GetService("Workspace")
local VirtualUser = game:GetService("VirtualUser")
local RunService = game:GetService("RunService")
local TeleportService = game:GetService("TeleportService") -- Добавлено для возможного использования

local player = Players.LocalPlayer
local Nplayer = player.Name
local character = player.Character or player.CharacterAdded:Wait()
local humanoid = character:WaitForChild("Humanoid")
local humanoidRootPart = character:WaitForChild("HumanoidRootPart")

-- Update character and humanoid references on respawn
player.CharacterAdded:Connect(function(newChar)
    character = newChar
    humanoid = character:WaitForChild("Humanoid")
    humanoidRootPart = character:WaitForChild("HumanoidRootPart")
end)

-- Main Rayfield window setup
local Window = Rayfield:CreateWindow({
    Name = "🌟Luna nursultan nextgen🌟",
    LoadingTitle = "🌟Luna nursultan nextgen🌟",
    LoadingSubtitle = "Привет! Я часть клиента 🌟Luna🌟!",
    Theme = "DarkBlue",
    DisableRayfieldPrompts = true,
    DisableBuildWarnings = true,
    ConfigurationSaving = {
       Enabled = false,
       FolderName = nil,
       FileName = "CHH Minimal"
    },
    Discord = {
       Enabled = false,
       Invite = "",
       RememberJoins = false
    },
    KeySystem = false,
    KeySettings = {
       Title = "CHHHHH",
       Subtitle = "Минимальный скрипт",
       Note = "Ключ не требуется для этой версии.",
       FileName = "CHHMinimalKey",
       SaveKey = true,
       GrabKeyFromSite = false,
       Key = {"minimal"}
    }
})

-- Создаем только вкладку Global
local Global = Window:CreateTab("Бизнес (авто фарм)")

----------------------------------------------------------------------------------------------------
-- === Анти-АФК логика ===
----------------------------------------------------------------------------------------------------
local antiAFKConnection = nil

local function enableAntiAFK()
    if not antiAFKConnection then
        Rayfield:Notify({
            Title = "Анти афк | ВКЛ",
            Content = "🌟Luna🌟",
            Duration = 6.5,
            Image = 124144713366592,
         })
        antiAFKConnection = player.Idled:Connect(function()
            VirtualUser:ClickButton2(Vector2.new()) -- Имитация клика мышкой
        end)
    end
end

local function disableAntiAFK()
    if antiAFKConnection then
        antiAFKConnection:Disconnect() -- Отключаем существующее соединение
        antiAFKConnection = nil         -- Сбрасываем ссылку на соединение
        Rayfield:Notify({
            Title = "Анти афк | ВЫКЛ",
            Content = "🌟Luna🌟",
            Duration = 6.5,
            Image = 124144713366592,
         })
    end
end


-- Анти-АФК теперь управляется только переключателем
-- Удалена функция antiAFKLoop, так как она избыточна

----------------------------------------------------------------------------------------------------
-- === Повороты логика ===
----------------------------------------------------------------------------------------------------
local rotationConnection = nil
local bodyGyro = nil

local function enableRotation()
    if rotationConnection then return end

    Rayfield:Notify({
        Title = "Повороты | ВКЛ",
        Content = "🌟Luna🌟",
        Duration = 6.5,
        Image = 124144713366592,
    })

    if not character.PrimaryPart:FindFirstChildOfClass("BodyGyro") then
        bodyGyro = Instance.new("BodyGyro")
        bodyGyro.MaxTorque = Vector3.new(math.huge, math.huge, math.huge)
        bodyGyro.P = 20000
        bodyGyro.D = 1250
        bodyGyro.Parent = character.PrimaryPart
    else
        bodyGyro = character.PrimaryPart:FindFirstChildOfClass("BodyGyro")
    end

    local currentAngle = character.PrimaryPart.Orientation.Y -- Начальный угол
    rotationConnection = RunService.Heartbeat:Connect(function(deltaTime)
        if not character or not character.PrimaryPart or not bodyGyro then return end

        currentAngle = currentAngle + (90 * deltaTime) -- Поворачиваем на 90 градусов в секунду
        bodyGyro.CFrame = CFrame.new(character.PrimaryPart.Position) * CFrame.Angles(0, math.rad(currentAngle), 0)
    end)
end

local function disableRotation()
    if not rotationConnection then return end

    Rayfield:Notify({
        Title = "Повороты | ВЫКЛ",
        Content = "🌟Luna🌟",
        Duration = 6.5,
        Image = 124144713366592,
    })

    if rotationConnection then
        rotationConnection:Disconnect()
        rotationConnection = nil
    end
    if bodyGyro then
        bodyGyro:Destroy()
        bodyGyro = nil
    end
end

----------------------------------------------------------------------------------------------------
-- === Авто-прыжок логика ===
----------------------------------------------------------------------------------------------------
local autoJumpCoroutine = nil

local function enableAutoJump()
    if autoJumpCoroutine then return end

    Rayfield:Notify({
        Title = "Авто-прыжок | ВКЛ",
        Content = "🌟Luna🌟",
        Duration = 6.5,
        Image = 124144713366592,
    })

    autoJumpCoroutine = task.spawn(function()
        while true do
            if not humanoid then break end
            if humanoid.FloorMaterial ~= Enum.Material.Air then
                humanoid:ChangeState(Enum.HumanoidStateType.Jumping)
            end
            task.wait(0.5)
        end
    end)
end

local function disableAutoJump()
    if not autoJumpCoroutine then return end

    Rayfield:Notify({
        Title = "Авто-прыжок | ВЫКЛ",
        Content = "🌟Luna🌟",
        Duration = 6.5,
        Image = 124144713366592,
    })

    task.cancel(autoJumpCoroutine)
    autoJumpCoroutine = nil
end

----------------------------------------------------------------------------------------------------
-- === Элементы GUI на вкладке Global ===
----------------------------------------------------------------------------------------------------

-- Переключатель Авто фарм
local AutoFarmToggle = Global:CreateToggle({
    Name = "Включить Авто фарм",
    CurrentValue = false,
    Flag = "AutoFarmToggle",
    Callback = function(Value)
        getgenv().AF = Value

        local function startAutoFarm()
            if not getgenv().AF then return end

            local currentCharacter = Players.LocalPlayer.Character or Players.LocalPlayer.CharacterAdded:Wait()
             if not currentCharacter then return end
            local currentHumanoidRootPart = currentCharacter:WaitForChild("HumanoidRootPart")
            if not currentHumanoidRootPart then return end

            local newPart = Instance.new("Part")
            newPart.Size = Vector3.new(5, 1, 5)
            newPart.Transparency = 1
            newPart.CanCollide = true
            newPart.Anchored = true
            newPart.Parent = Workspace

            local decal = Instance.new("Decal")
            decal.Texture = "rbxassetid://139953968294114"
            decal.Face = Enum.NormalId.Top
            decal.Parent = newPart

            local function TPAF(iteration)
                if not getgenv().AF then return end
                if not Players.LocalPlayer.Character or not Players.LocalPlayer.Character:FindFirstChild("HumanoidRootPart") then return end -- Добавлена проверка на существование персонажа и HRP
                currentHumanoidRootPart = Players.LocalPlayer.Character:FindFirstChild("HumanoidRootPart") -- Обновление ссылки на HRP на всякий случай
                
                if iteration == 5 then
                    firetouchinterest(Players.LocalPlayer.Character:WaitForChild("HumanoidRootPart"), Workspace.BoatStages.NormalStages.TheEnd.GoldenChest.Trigger, 0)
                    task.delay(0.8, function()
                        Workspace.ClaimRiverResultsGold:FireServer()
                    end)
                    currentHumanoidRootPart.CFrame = CFrame.new(-51, 65, 984 + (iteration - 1) * 770)
                else
                    if iteration == 1 then
                        currentHumanoidRootPart.CFrame = CFrame.new(160.16104125976562, 29.595888137817383, 973.813720703125)
                    else
                        currentHumanoidRootPart.CFrame = CFrame.new(-51, 65, 984 + (iteration - 1) * 770)
                    end
                end
                newPart.Position = currentHumanoidRootPart.Position - Vector3.new(0, 2, 0)

                task.wait(2.0)
                if iteration == 1 then
                    task.wait(2.0) -- Дополнительная задержка для первого этапа
                end
                if iteration ~= 4 then -- Только если не 4-я итерация
                    Workspace.ClaimRiverResultsGold:FireServer()
                end
            end

            for i = 1, 10 do
                if not getgenv().AF then
                    break
                end
                TPAF(i)
            end

            newPart:Destroy()
        end

        local charAddedConnection
        local function onRespawned()
            if getgenv().AF == true then
                local currentCharacter = Players.LocalPlayer.Character or Players.LocalPlayer.CharacterAdded:Wait()
                if not currentCharacter then return end
                currentCharacter:WaitForChild("HumanoidRootPart")
                startAutoFarm()
            end
        end

        if Value then
            Rayfield:Notify({
                Title = "Авто фарм | ВКЛ",
                Content = "💴🌟Luna🌟💰",
                Duration = 6.5,
                Image = 124144713366592,
             })
             if Players.LocalPlayer.Character then -- Добавлена проверка11
                 Players.LocalPlayer.Character:BreakJoints()
             end
            task.wait(1)
            charAddedConnection = Players.LocalPlayer.CharacterAdded:Connect(onRespawned)
            -- Запускаем автофарм сразу при включении, если персонаж уже жив
            if Players.LocalPlayer.Character and Players.LocalPlayer.Character:FindFirstChild("HumanoidRootPart") then
                startAutoFarm()
            end
        else
            Rayfield:Notify({
                Title = "Авто фарм | ВЫКЛ",
                Content = "",
                Duration = 6.5,
                Image = 124144713366592,
             })
            if charAddedConnection then
                charAddedConnection:Disconnect()
                charAddedConnection = nil
            end
            getgenv().AF = false -- Убедимся, что AF переменная выключена
        end
    end,
})

-- Переключатель Анти афк
local AFKToggle = Global:CreateToggle({
    Name = "Анти афк",
    CurrentValue = false,
    Flag = "AntiAfkToggle",
    Callback = function(Value)
        getgenv().afk6464 = Value
        if Value then
            enableAntiAFK()
        else
            disableAntiAFK()
        end
    end,
})

if getgenv().afk6464 == true then
    AFKToggle:Set(true)
end

-- Переключатель Повороты
local RotationToggle = Global:CreateToggle({
    Name = "Повороты",
    CurrentValue = false,
    Flag = "RotationToggle",
    Callback = function(Value)
        if Value then
            enableRotation()
        else
            disableRotation()
        end
    end,
})

-- Переключатель Авто-прыжок
local AutoJumpToggle = Global:CreateToggle({
    Name = "Авто-прыжок",
    CurrentValue = false,
    Flag = "AutoJumpToggle",
    Callback = function(Value)
        if Value then
            enableAutoJump()
        else
            disableAutoJump()
        end
    end,
})

-- === Статистика ===
----------------------------------------------------------------------------------------------------
local FStats = Global:CreateParagraph({Title = "Статистика", Content = "Время работы: -\nДошёл до конца: -\nЗолота получено: -\n\nЗолота в час: -"})

local clockTime = 0
local runningStatsClock = false
local totalGoldGained = 0
local Ftime = "0 часов 0 минут 0 секунд"
local totalGoldBlock = 0
local GoldPerSecond = 0
local GoldPerMinute = 0
local GoldPerHour = 0
local lastGoldValue
local IGBLOCK

-- Инициализируем lastGoldValue и IGBLOCK после того, как player.Data доступно
local function initializeStats()
    while not Players.LocalPlayer.Data or not Players.LocalPlayer.Data:FindFirstChild("Gold") or not Players.LocalPlayer.Data:FindFirstChild("GoldBlock") do
        task.wait(0.1)
    end
    lastGoldValue = Players.LocalPlayer.Data.Gold.Value
    IGBLOCK = Players.LocalPlayer.Data.GoldBlock.Value
end
initializeStats()

local function formatTime(seconds)
    local hours = math.floor(seconds / 3600)
    local minutes = math.floor((seconds % 3600) / 60)
    local sec = math.floor(seconds % 60)
    return hours .. " часов " .. minutes .. " минут " .. sec .. " секунд"
end

local function startStatsClockTask()
    if runningStatsClock then return end
    runningStatsClock = true

    while runningStatsClock do
        if getgenv().AF then
            clockTime = clockTime + 0.5
        else
            runningStatsClock = false
        end
        task.wait(0.5)
    end
end

-- Запуск часов статистики при активации автофарма
RunService.Stepped:Connect(function()
    if getgenv().AF and not runningStatsClock then
        task.delay(5, startStatsClockTask)
    end
end)

local function initClockUpdate()
    while true do
        if not Players.LocalPlayer or not Players.LocalPlayer.Data or not Players.LocalPlayer.Data:FindFirstChild("Gold") then
            task.wait(0.5)
            continue
        end

        local FinalGold = Players.LocalPlayer.Data.Gold.Value
        Ftime = formatTime(clockTime)

        local GoldGainedThisCycle = FinalGold - lastGoldValue
        totalGoldGained = totalGoldGained + GoldGainedThisCycle

        if Players.LocalPlayer.Data:FindFirstChild("GoldBlock") then
            local FGBLOCK = Players.LocalPlayer.Data.GoldBlock.Value
            totalGoldBlock = FGBLOCK - IGBLOCK
        end


        if clockTime > 0 then
            GoldPerSecond = totalGoldGained  / clockTime
            GoldPerMinute = GoldPerSecond * 60
            GoldPerHour = GoldPerSecond * 3600
        else
            GoldPerSecond = 0
            GoldPerMinute = 0
            GoldPerHour = 0
        end

        FStats:Set({
            Title = "Статистика",
            Content = "Время работы: " .. Ftime .. "\n" ..
                      "Дошёл до конца: " .. totalGoldBlock .. " раз\n" ..
                      "Золота получено: " .. math.floor(totalGoldGained) .. "\n" ..
                      "Золота в час: " .. math.floor(GoldPerHour) .. "\n" ..
                      "Золота в минуту: " .. math.floor(GoldPerMinute) .. "\n" ..
                      "Золота в секунду: " .. string.format("%.2f", GoldPerSecond) .. "\n\n" ..
                      "--- Информация ---\n" ..
                      "Телеграм создателей:\n\n" ..
                      "👾 Разработчик: t.me/Im_DrHacker 👾\n" ..
                      "Дата создания: 26.08.2025 все права зафырканы 🦔\n\n" ..
                      "СПИСОК ОБНОВЛЕНИЯ:\n\n" ..
                      "При обычном фарме в конце персонаж который должен умереть чтобы вернуться назад вовсе плавал в воде\n" ..
                      "Решение: добавить автопрыжки чтобы увеличить шанс добавления золота и возвращения персонажа назад\n" ..
                      "Обновлено: 15.07.2026\n\n" ..
                      "Добавлена слежка за балансом в реальном времени\n" ..
                      "Изменено: Обновление каждые 0.5 секунд\n" ..
                      "Добавлено: 15.07.2026\n\n" ..
                      "Планируется оффициальный переезд на библиотеку Luna client",
        })

        lastGoldValue = FinalGold
        task.wait(0.5)
    end
end

GoldPerHour = 0
GoldPerMinute = 0
GoldPerSecond = 0

task.spawn(initClockUpdate)
