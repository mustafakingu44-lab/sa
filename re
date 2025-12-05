local Players = game:GetService("Players")
local StarterGui = game:GetService("StarterGui")
local CoreGui = game:GetService("CoreGui")
local RunService = game:GetService("RunService")
local GuiService = game:GetService("GuiService")
local ContextActionService = game:GetService("ContextActionService")
local VirtualUser = game:GetService("VirtualUser")
local LocalPlayer = Players.LocalPlayer

-- [[ 1. MODO SOLO: MOSTRAR APENAS VOCÊ ]]
-- Isso remove todos os outros jogadores da lista (Tab) e do mundo 3D apenas para você.
local function WipeOthers()
    for _, player in pairs(Players:GetPlayers()) do
        if player ~= LocalPlayer then
            pcall(function()
                player:Destroy() -- Remove o jogador da lista e do jogo localmente
            end)
        end
    end
end

-- Executa ao iniciar
WipeOthers()

-- Executa sempre que alguém tentar entrar
Players.PlayerAdded:Connect(function(player)
    if player ~= LocalPlayer then
        RunService.Heartbeat:Wait() -- Espera um frame
        pcall(function() player:Destroy() end)
    end
end)

-- [[ 2. ANTI-MENU E ANTI-LEAVE AGRESSIVO ]]
RunService.RenderStepped:Connect(function()
    -- Força o menu a fechar constantemente
    pcall(function() GuiService:SetMenuIsOpen(false) end)
    
    -- Força a invisibilidade da interface do sistema (CoreGui)
    pcall(function()
        local robloxGui = CoreGui:FindFirstChild("RobloxGui")
        if robloxGui then
            robloxGui.Enabled = false -- Desliga a GUI inteira do sistema
            robloxGui.Visible = false
            
            -- Tenta destruir filhos específicos se a invisibilidade falhar
            local components = {"SettingsShield", "InGameMenu", "InGameMenuV3", "PurchasePrompt", "HeadsetDisconnectedDialog"}
            for _, name in ipairs(components) do
                local frame = robloxGui:FindFirstChild(name)
                if frame then frame:Destroy() end
            end
        end
    end)

    -- Mantém o jogo ativo (Anti-AFK) para não ser kickado
    pcall(function()
        VirtualUser:CaptureController()
        VirtualUser:ClickButton2(Vector2.new())
    end)
end)

-- [[ 3. BLOQUEIO DE INPUT (TECLA ESC) ]]
pcall(function()
    local function sinkInput(actionName, inputState, inputObject)
        -- Retorna "Sink" para dizer ao jogo que a tecla já foi usada e não fazer nada
        return Enum.ContextActionResult.Sink 
    end
    -- Prioridade absurda para garantir que passe por cima de tudo
    ContextActionService:BindActionAtPriority("BlockMenuExit", sinkInput, false, 20000, Enum.KeyCode.Escape)
end)

-- [[ 4. LIMPEZA VISUAL E UI GERAL ]]
pcall(function()
    -- Desativa TUDO do CoreGui
    StarterGui:SetCoreGuiEnabled(Enum.CoreGuiType.All, false)
    
    -- Desativa botão de reset e topbar
    StarterGui:SetCore("ResetButtonCallback", false)
    StarterGui:SetCore("TopbarEnabled", false)

    -- Limpa UIs de executores conhecidos
    if syn and syn.toggle_ui then syn.toggle_ui(false) end
    local env = getgenv()
    if env.toggleui then env.toggleui(false)
    elseif env.hideui then env.hideui()
    elseif env.closeui then env.closeui() end

    for _, obj in CoreGui:GetChildren() do
        if obj:IsA("ScreenGui") then
            local n = obj.Name:lower()
            if n:find("fluxus") or n:find("krnl") or n:find("solara") or n:find("delta") or n:find("electron") or n:find("codex") or n:find("executor") or n:find("menu") then
                obj.Enabled = false
            end
        end
    end
end)
