import React from 'react';

export type IPageNames = 'mainmenu' | 'quiz-selection' | 'devices' | 'settings';

export type Props = {
  setCurrentPage: React.Dispatch<React.SetStateAction<IPageNames>>;
};

export type DatabaseQuery =
  | {
      requestFor: 'quizzes';
    }
  | {
      requestFor: 'questions';
      quizId: Number;
    };

export interface Device {} // TODO: fix this as well

export interface Answer {
  id: Number | null;
  device_id: Number;
  question_id: Number;
  room_id: Number;
  timestamp: Date;
  answer: string; // TODO: make better fucking name for this
}

export interface Question {
  id: Number | null;
  quiz_id: Number;
  answer_1: string;
  answer_2: string;
  answer_3: string;
  answer_4: string;
  answer_1_valid: boolean;
  answer_2_valid: boolean;
  answer_3_valid: boolean;
  answer_4_valid: boolean;
}

export interface Quiz {
  id: Number | null;
  title: string;
  description: string;
  created_at: Date;
  updated_at: Date;
}

export interface Room {
  id: Number | null;
  quiz_id: Number;
  started: Date;
}
