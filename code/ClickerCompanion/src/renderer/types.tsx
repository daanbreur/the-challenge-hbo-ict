import React from 'react';

export type IPageNames = 'mainmenu' | 'quiz-selection' | 'devices' | 'settings';

export type Props = {
  setCurrentPage: React.Dispatch<React.SetStateAction<IPageNames>>;
};
