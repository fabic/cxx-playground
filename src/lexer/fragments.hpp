
  /**
   */
  class Fragment {
    protected:
      std::shared_ptr<Fragment> previous_;
      std::shared_ptr<Fragment> next_;
      unsigned int _line, _colunm;
      // list of lexed tokens with no semantics ?
  };


  /**
   */
  class Block : public Fragment {
    protected:
      std::shared_ptr<Fragment> inner_;
  };
