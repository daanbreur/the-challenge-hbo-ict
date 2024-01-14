import '../../globals.css';
import '../../fonts.css';
import 'tailwindcss/tailwind.css';
import React from 'react';
import { Props } from '../../types';

import MenuWrapper from '../../components/menuview/MenuWrapper';
import ReportList from '../../components/reports/ReportList';
import Report from '../../components/reports/Report';

export default function Reports({ setCurrentPage }: Props) {
  return (
    <MenuWrapper setCurrentPage={setCurrentPage}>
      <ReportList name="Reports">
        <Report name="Programming 2" time={new Date(1704921355000)} players={10}/>

      </ReportList>
    </MenuWrapper>
  );
}
