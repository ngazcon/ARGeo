package vyglab.argeo.app.controller;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by root on 21/09/16.
 */
public class TabSectionsPagerAdapter extends FragmentPagerAdapter {
    private final List<Fragment> m_Fragments = new ArrayList<>();
    private final List<String> m_FragmentsTitles = new ArrayList<>();

    public TabSectionsPagerAdapter(FragmentManager fm){
        super(fm);
    }

    @Override
    public Fragment getItem(int position){
        return m_Fragments.get(position);
    }

    @Override
    public int getCount(){
        return m_Fragments.size();
    }

    public void addFragment(Fragment fragment, String title){
        m_Fragments.add(fragment);
        m_FragmentsTitles.add(title);
    }

    @Override
    public CharSequence getPageTitle(int position){
        return m_FragmentsTitles.get(position);
    }
}