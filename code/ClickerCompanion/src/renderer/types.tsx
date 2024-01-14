import React from 'react';

export type IPageNames = 'home' | 'quizzes' | 'reports' | 'settings';

export type IDatabaseQuery =
  | {
      type: 'request';
      requestFor: 'quizzes';
    }
  | {
      type: 'request';
      requestFor: 'quiz';
      quizId: Number;
    }
  | {
      type: 'request';
      requestFor: 'questions';
      quizId: Number;
    }
  | {
      type: 'request';
      requestFor: 'question';
      questionId: Number;
    };

export interface IDevice {} // TODO: fix this as well

export interface IAnswer {
  id: Number | null;
  device_id: Number;
  question_id: Number;
  room_id: Number;
  timestamp: Date;
  answer: string; // TODO: make better fucking name for this
}

export interface IQuestion {
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

export interface IQuiz {
  id: Number | null;
  title: string;
  description: string;
  created_at: Date;
  updated_at: Date;
}

export interface IRoom {
  id: Number | null;
  quiz_id: Number;
  started: Date;
}

export type Props = {
  setCurrentPage: React.Dispatch<React.SetStateAction<IPageNames>>;
  // setSelectedQuiz: React.Dispatch<React.SetStateAction<IQuiz>>;
  // selectedQuiz: IQuiz;
};
