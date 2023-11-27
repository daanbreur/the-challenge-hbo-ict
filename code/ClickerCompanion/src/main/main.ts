/* eslint global-require: off, no-console: off, promise/always-return: off */

/**
 * This module executes inside of electron's main process. You can start
 * electron renderer process from here and communicate with the other processes
 * through IPC.
 *
 * When running `npm run build` or `npm run build:main`, this file is compiled to
 * `./src/main.js` using webpack. This gives us some performance wins.
 */
import path from 'path';
import { app, BrowserWindow, shell, ipcMain, dialog } from 'electron';
import { autoUpdater } from 'electron-updater';
import log from 'electron-log';
import sqlite from 'sqlite3';
import MenuBuilder from './menu';
import { resolveHtmlPath } from './util';
import { IDatabaseQuery } from '../renderer/types';

class AppUpdater {
  constructor() {
    log.transports.file.level = 'info';
    autoUpdater.logger = log;
    autoUpdater.checkForUpdatesAndNotify();
  }
}

const sqlite3 = sqlite.verbose();
const db = new sqlite3.Database(
  path.join(app.getPath('userData'), 'database.db'),
);

db.serialize(() => {
  db.run(
    'CREATE TABLE IF NOT EXISTS `quizzes` (`id` integer PRIMARY KEY,`title` varchar(255),`description` varchar(255),`created_at` timestamp,`updated_at` timestamp)',
  );
  db.run(
    'CREATE TABLE IF NOT EXISTS `questions` (`id` integer PRIMARY KEY,`quiz_id` integer,`answer_1` varchar(255),`answer_2` varchar(255),`answer_3` varchar(255),`answer_4` varchar(255),`answer_1_valid` bool,`answer_2_valid` bool,`answer_3_valid` bool,`answer_4_valid` bool,FOREIGN KEY (`quiz_id`) REFERENCES `quizzes` (`id`))',
  );
  db.run(
    'CREATE TABLE IF NOT EXISTS `answers` (`id` integer PRIMARY KEY,`device_id` integer,`question_id` integer,`room_id` integer,`timestamp` timestamp,`answer` varchar(255),FOREIGN KEY (`question_id`) REFERENCES `questions` (`id`),FOREIGN KEY (`device_id`) REFERENCES `devices` (`id`),FOREIGN KEY (`room_id`) REFERENCES `rooms` (`id`))',
  );
  db.run(
    'CREATE TABLE IF NOT EXISTS `devices` (`id` integer PRIMARY KEY,`username` varchar(255))',
  );
  db.run(
    'CREATE TABLE IF NOT EXISTS `rooms` (`id` integer PRIMARY KEY,`quiz_id` integer,`started` timestamp,FOREIGN KEY (`quiz_id`) REFERENCES `quizzes` (`id`))',
  );
});

let mainWindow: BrowserWindow | null = null;

ipcMain.on('ipc-example', async (event, arg) => {
  const msgTemplate = (pingPong: string) => `IPC test: ${pingPong}`;
  console.log(msgTemplate(arg));
  event.reply('ipc-example', msgTemplate('pong'));
});

ipcMain.on('database-communication', async (event, data: IDatabaseQuery) => {
  switch (data.requestFor) {
    case 'quizzes':
      db.all('SELECT * FROM quizzes q;', (err: any, rows: any) => {
        event.reply('database-communication:quizzes', rows);
      });
      break;

    case 'questions':
      db.all(
        'SELECT * FROM questions q WHERE quiz_id = $id',
        {
          $id: data.quizId,
        },
        (err: any, rows: any) => {
          event.reply('database-communication:questions', rows);
        },
      );
      break;

    default:
      event.reply('database-communication', null);
      break;
  }
});

if (process.env.NODE_ENV === 'production') {
  const sourceMapSupport = require('source-map-support');
  sourceMapSupport.install();
}

const isDebug =
  process.env.NODE_ENV === 'development' || process.env.DEBUG_PROD === 'true';

if (isDebug) {
  require('electron-debug')();
}

const installExtensions = async () => {
  const installer = require('electron-devtools-installer');
  const forceDownload = !!process.env.UPGRADE_EXTENSIONS;
  const extensions = ['REACT_DEVELOPER_TOOLS'];

  return installer
    .default(
      extensions.map((name) => installer[name]),
      forceDownload,
    )
    .catch(console.log);
};

const createWindow = async () => {
  if (isDebug) {
    await installExtensions();
  }

  const RESOURCES_PATH = app.isPackaged
    ? path.join(process.resourcesPath, 'assets')
    : path.join(__dirname, '../../assets');

  const getAssetPath = (...paths: string[]): string => {
    return path.join(RESOURCES_PATH, ...paths);
  };

  mainWindow = new BrowserWindow({
    show: false,
    width: 1366,
    minWidth: 600,
    height: 768,
    minHeight: 768,
    icon: getAssetPath('icon.png'),
    webPreferences: {
      preload: app.isPackaged
        ? path.join(__dirname, 'preload.js')
        : path.join(__dirname, '../../.erb/dll/preload.js'),
    },
  });

  mainWindow.loadURL(resolveHtmlPath('index.html'));

  mainWindow.on('ready-to-show', () => {
    if (!mainWindow) {
      throw new Error('"mainWindow" is not defined');
    }
    if (process.env.START_MINIMIZED) {
      mainWindow.minimize();
    } else {
      mainWindow.show();
    }
  });

  mainWindow.on('closed', () => {
    mainWindow = null;
  });

  const menuBuilder = new MenuBuilder(mainWindow);
  menuBuilder.buildMenu();

  // Open urls in the user's browser
  mainWindow.webContents.setWindowOpenHandler((edata) => {
    shell.openExternal(edata.url);
    return { action: 'deny' };
  });

  mainWindow.webContents.session.on(
    'select-serial-port',
    (event, portList, webContents, callback) => {
      // Add listeners to handle ports being added or removed before the callback for `select-serial-port`
      // is called.

      console.log(portList);
      if (portList && portList.length > 0) {
        const id = dialog.showMessageBoxSync({
          title: 'ClickerCompanion',
          message: 'Please pick the correct serial device',
          buttons: portList.map((a) => `${a.displayName} (${a.portName})`),
        });
        console.log(portList[id]);
        callback(portList[id].portId);
      } else {
        callback(''); // Could not find any matching devices
      }
    },
  );

  // Remove this if your app does not use auto updates
  // eslint-disable-next-line
  new AppUpdater();
};

/**
 * Add event listeners...
 */

app.on('window-all-closed', () => {
  db.close();
  // Respect the OSX convention of having the application in memory even
  // after all windows have been closed
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app
  .whenReady()
  .then(() => {
    createWindow();
    app.on('activate', () => {
      // On macOS it's common to re-create a window in the app when the
      // dock icon is clicked and there are no other windows open.
      if (mainWindow === null) createWindow();
    });
  })
  .catch(console.log);
