const { app, BrowserWindow } = require('electron')

const createWindow = () => {
    const win = new BrowserWindow({
        width: 1600,
        height: 900,
        icon: 'images/icons/icon.png',
        backgroundMaterial: "mica",
        autoHideMenuBar: true,
        resizable: true,
    })

    win.loadFile('index.html').then()

}
app.whenReady().then(() => {
    createWindow()
})