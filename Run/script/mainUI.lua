MainUI = CWnd:new();
LoginUI = CWnd:new();
function createMainUI()
	--MainUI:setPosition(0, 0);
	--MainUI:setSize(800, 600);
end
	
function MainUI:ProcessMsg()
	self:setPosition(10, 0);
end

function LoginUI:ProcessMsg()
	self:setPosition(0, 0);
end

function LoginUI:WndProcess(nId)
	self:setSize(500, 500);	
end


gMainUI = MainUI;
gLoginUI = LoginUI;


