import { createRoot } from 'react-dom/client';
// eslint-disable-next-line import/no-cycle
import App from './App';
import { DatabaseQuery, Question, Quiz } from './types';

const container = document.getElementById('root') as HTMLElement;
const root = createRoot(container);
root.render(<App />);

// calling IPC exposed from preload script
window.electron.ipcRenderer.once('ipc-example', (arg) => {
  // eslint-disable-next-line no-console
  console.log(arg);
});
window.electron.ipcRenderer.sendMessage('ipc-example', ['ping']);

export async function getQuizzes(): Promise<Quiz[]> {
  return new Promise((resolve) => {
    window.electron.ipcRenderer.once(
      'database-communication:quizzes',
      (arg) => {
        const data: Quiz[] = arg as Quiz[];
        resolve(data);
      },
    );
    window.electron.ipcRenderer.sendMessage('database-communication', {
      requestFor: 'quizzes',
    } as DatabaseQuery);
  });
}

export async function getQuizQuestions(quizId: Number): Promise<Question[]> {
  return new Promise((resolve) => {
    window.electron.ipcRenderer.once(
      'database-communication:questions',
      (arg) => {
        const data: Question[] = arg as Question[];
        resolve(data);
      },
    );
    window.electron.ipcRenderer.sendMessage('database-communication', {
      requestFor: 'questions',
      quizId,
    } as DatabaseQuery);
  });
}

// async function getQuizes(data: String) {
//   return new Promise((resolve) => {
//     window.electron.ipcRenderer.once('database-communication', (arg) => {
//       resolve(arg);
//     });
//     window.electron.ipcRenderer.sendMessage('database-communication', data);
//   });
// }
