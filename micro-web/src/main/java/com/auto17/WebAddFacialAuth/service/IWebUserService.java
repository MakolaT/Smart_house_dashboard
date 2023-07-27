package com.auto17.WebAddFacialAuth.service;

import java.util.List;
import com.auto17.WebAddFacialAuth.domain.WebUser;

/**
 * webUserService interface
 *
 * @author jianxiang sun
 * @date 2023-03-04
 */
public interface IWebUserService
{
    /**
     *
     * @param userId webUserID
     * @return webUser
     */
    public WebUser selectWebUserById(Long userId);

    public WebUser selectWebUserOne(WebUser webUser);
    /**
     *
     * @param webUser webUser
     * @return webUser List
     */
    public List<WebUser> selectWebUserList(WebUser webUser);

    /**
     *
     * @param webUser webUser
     * @return int
     */
    public int insertWebUser(WebUser webUser);

    /**
     *
     * @param webUser webUser
     * @return int
     */
    public int updateWebUser(WebUser webUser);

    /**
     * 删除webUser信息
     *
     * @param userId webUserID
     * @return int
     */
    public int deleteWebUserById(Long userId);
}
