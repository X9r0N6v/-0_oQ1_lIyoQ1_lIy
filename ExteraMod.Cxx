local ShtermoBiblio = loadstring(game:HttpGet(('https://raw.githubusercontent.com/SPTWondershaew/RobloxHacker/refs/heads/main/FreezeA')))()

local Window = ShtermoBiblio:MakeWindow({Name = "👑Luna War👑", HidePremium = false, SaveConfig = true, ConfigFolder = "👑Luna client👑"})

local MainTab = Window:MakeTab({
 Name = "Меню",
 Icon = "rbxassetid://4483345998",
 PremiumOnly = false
})

local Section = MainTab:AddSection({
 Name = "👑Creator t.me/Im_DrHacker👑"
})

-- Переменные для управления Киллаурой
local killAllActive = false
local killAllConnection = nil

MainTab:AddDropdown({
 Name = "Киллаура",
 Default = "",
 Options = {"Убивать всех"}, -- Оставлена только опция "Kill all"
 Callback = function(Value)
  if Value == "Убивать всех" then
      if not killAllActive then -- Активируем, если сейчас не активна
          killAllActive = true
          ShtermoBiblio:MakeNotification({
             Name = "👑Luna client👑",
             Content = "Киллаура активирована",
             Image = "rbxassetid://1633620069",
             Time = 10
          })

          killAllConnection = game:GetService("RunService").RenderStepped:Connect(function()
              local player = game:GetService("Players").LocalPlayer
              local playersList = game.Players:GetPlayers()
              for i = 2, #playersList do
                  local targetCharacter = playersList[i].Character
                  if targetCharacter then
                      local tool = player.Character and player.Character:FindFirstChildOfClass("Tool")
                      if tool and tool:FindFirstChild("Handle") then
                          tool:Activate()
                          for _, part in next, targetCharacter:GetChildren() do
                              if part:IsA("BasePart") then
                                  firetouchinterest(tool.Handle, part, 0)
                                  firetouchinterest(tool.Handle, part, 1)
                              end
                          end
                      end
                  end
              end
          end)
      else -- Если уже активна и выбрана снова, деактивируем (поведение переключателя)
          killAllActive = false
          if killAllConnection then
              killAllConnection:Disconnect()
              killAllConnection = nil
          end
          ShtermoBiblio:MakeNotification({
             Name = "👑Luna client👑",
             Content = "Киллаура активирована",
             Image = "rbxassetid://1633620069",
             Time = 5
          })
      end
  end
 end
})
-- Кнопка "ESP" удалена
MainTab:AddButton({
 Name = "Подсветка игроков",
 Callback = function()
        loadstring(game:HttpGet('https://raw.githubusercontent.com/SPTWondershaew/RobloxHacker/refs/heads/main/FreezeV'))()
   end
})




MainTab:AddButton({
 Name = "Подсвечивание игроков",
 Callback = function()

local Players = game:GetService("Players")
local LocalPlayer = Players.LocalPlayer

-- UI TextLabel
local screenGui = Instance.new("ScreenGui")
screenGui.Name = "CreditGui"
screenGui.ResetOnSpawn = false
screenGui.Parent = LocalPlayer:WaitForChild("PlayerGui")

-- Highlight function
local function highlightPlayer(player)
	if player == LocalPlayer then return end

	player.CharacterAdded:Connect(function(character)
		character:WaitForChild("HumanoidRootPart")
		local highlight = Instance.new("Highlight")
		highlight.FillColor = Color3.fromRGB(0, 255, 0)
		highlight.OutlineColor = Color3.fromRGB(255, 255, 255)
		highlight.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
		highlight.Adornee = character
		highlight.Parent = character
	end)

	if player.Character then
		local highlight = Instance.new("Highlight")
		highlight.FillColor = Color3.fromRGB(0, 255, 0)
		highlight.OutlineColor = Color3.fromRGB(255, 255, 255)
		highlight.DepthMode = Enum.HighlightDepthMode.AlwaysOnTop
		highlight.Adornee = player.Character
		highlight.Parent = player.Character
	end
end

for _, player in pairs(Players:GetPlayers()) do
	highlightPlayer(player)
end

Players.PlayerAdded:Connect(highlightPlayer)
   end
})


