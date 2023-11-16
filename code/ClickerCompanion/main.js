const { app, BrowserWindow } = require('electron')

const createWindow = () => {
    const win = new BrowserWindow({
        width: 1600,
        height: 900,
        icon: 'images/icons/icon.png'
    })

    win.loadFile('index.html')
}
app.whenReady().then(() => {
    createWindow()
})