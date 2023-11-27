import { createRoot } from 'react-dom/client';
// eslint-disable-next-line import/no-cycle
import App from './App';
import { IDatabaseQuery, IQuestion, IQuiz } from './types';

const container = document.getElementById('root') as HTMLElement;
const root = createRoot(container);
root.render(<App />);

// calling IPC exposed from preload script
window.electron.ipcRenderer.once('ipc-example', (arg) => {
  // eslint-disable-next-line no-console
  console.log(arg);
});
window.electron.ipcRenderer.sendMessage('ipc-example', ['ping']);

export async function getQuizzes(): Promise<IQuiz[]> {
  return new Promise((resolve) => {
    window.electron.ipcRenderer.once(
      'database-communication:quizzes',
      (arg) => {
        const data: IQuiz[] = arg as IQuiz[];
        resolve(data);
      },
    );
    window.electron.ipcRenderer.sendMessage('database-communication', {
      requestFor: 'quizzes',
    } as IDatabaseQuery);
  });
}

export async function getQuizQuestions(quizId: Number): Promise<IQuestion[]> {
  return new Promise((resolve) => {
    window.electron.ipcRenderer.once(
      'database-communication:questions',
      (arg) => {
        const data: IQuestion[] = arg as IQuestion[];
        resolve(data);
      },
    );
    window.electron.ipcRenderer.sendMessage('database-communication', {
      requestFor: 'questions',
      quizId,
    } as IDatabaseQuery);
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
